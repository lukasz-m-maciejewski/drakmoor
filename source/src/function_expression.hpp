#pragma once


#include <algorithm>
#include <functional>
#include <iosfwd>
#include <map>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

namespace drakmoor
{

using base_type = double;
using arg_map = std::map<std::string, base_type>;

class expression;
class expression_visitor;

class atom
{
public:
    virtual ~atom() = default;
    virtual base_type eval_at(const arg_map&) const = 0;
    virtual std::unique_ptr<atom> clone() const = 0;
    virtual void accept(expression_visitor&) const = 0;
};

class constant : public atom
{
public:
    constant(base_type v_init) : v{v_init}
    {
    }

    base_type eval_at(const arg_map&) const override
    {
        return v;
    }

    std::unique_ptr<atom> clone() const override
    {
        return std::make_unique<constant>(*this);
    }

    void accept(expression_visitor& ev) const override;

    base_type value() const { return v; }

private:
    base_type v;
};

class placeholder : public atom
{
public:
    placeholder(std::string id_init) : id{std::move(id_init)}
    {
    }

    base_type eval_at(const arg_map& point) const override
    {
        return point.at(id);
    }

    std::unique_ptr<atom> clone() const override
    {
        return std::make_unique<placeholder>(*this);
    }

    void accept(expression_visitor& ev) const override;

    const std::string& label() const { return id; }

private:
    std::string id;
};

using operation_t = std::pair<std::function<base_type(base_type, base_type)>, std::string_view>;

class compound : public atom
{
public:
    compound(operation_t operation_init, std::unique_ptr<atom> v1,
             std::unique_ptr<atom> v2)
        : operation{std::move(operation_init)}
    {
        values.emplace_back(v1->clone());
        values.emplace_back(v2->clone());
    }

    compound(const compound& other) : operation{other.operation}
    {
        values.reserve(other.values.size());
        std::transform(other.values.begin(), other.values.end(),
                       std::back_inserter(values),
                       [](const auto& other_value) { return other_value->clone(); });
    }

    base_type eval_at(const arg_map& point) const override
    {
        if (values.empty())
        {
            throw std::logic_error{"no values in compound"};
        }

        std::vector<base_type> evaluated_values;
        evaluated_values.reserve(values.size());

        std::transform(values.begin(), values.end(), std::back_inserter(evaluated_values),
                       [&](const auto& v) { return v->eval_at(point); });

        return std::accumulate(evaluated_values.begin() + 1, evaluated_values.end(),
                               *evaluated_values.begin(), std::get<0>(operation));
    }

    std::unique_ptr<atom> clone() const override
    {
        return std::make_unique<compound>(*this);
    }

    void accept(expression_visitor& ev) const override;

    std::string_view get_operation_label() const
    {
        return std::get<1>(operation);
    }

    const auto& get_atoms() const
    {
        return values;
    }

private:
    operation_t operation;
    std::vector<std::unique_ptr<atom>> values;
};

class expression
{
public:
    expression(base_type c) : v{std::make_unique<constant>(c)}
    {
    }

    expression(std::string id) : v{std::make_unique<placeholder>(id)}
    {
    }

    expression(operation_t op, expression e1, expression e2)
        : v{std::make_unique<compound>(op, std::move(e1.v), std::move(e2.v))}
    {
    }

    base_type eval_at(const arg_map& point)
    {
        return v->eval_at(point);
    }

    void accept(expression_visitor& ev) const
    {
        v->accept(ev);
    }

    std::unique_ptr<atom> v;
};

inline expression operator+(expression e1, expression e2)
{
    return expression({std::plus<base_type>{}, "+"}, std::move(e1), std::move(e2));
}

inline expression operator-(expression e1, expression e2)
{
    return expression({std::minus<base_type>{}, "-"}, std::move(e1), std::move(e2));
}

inline expression operator*(expression e1, expression e2)
{
    return expression({std::multiplies<base_type>{}, "*"}, std::move(e1), std::move(e2));
}

inline expression operator/(expression e1, expression e2)
{
    return expression({std::divides<base_type>{}, "/"}, std::move(e1), std::move(e2));
}

class expression_visitor
{
public:
    virtual ~expression_visitor() noexcept = default;

    virtual void visit(const constant&) = 0;
    virtual void visit(const placeholder&)  = 0;
    virtual void visit(const compound&)  = 0;
};

class printer : public expression_visitor
{
public:
    printer(std::ostream&);

    void visit(const constant&) override;
    void visit(const placeholder&) override;
    void visit(const compound&) override;

private:
    std::ostream& os;
};

}
