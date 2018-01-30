#ifndef DRAKMOOR_FUNCTION_EXPRESSION
#define DRAKMOOR_FUNCTION_EXPRESSION

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <numeric>
#include <vector>
#include <string>

using base_type = double;
using arg_map = std::map<std::string, base_type>;

class expression;

class value
{
public:
    virtual ~value() = default;
    virtual base_type eval_at(const arg_map&) const = 0;
    virtual std::unique_ptr<value> clone() const = 0;
};

class constant : public value
{
public:
    constant(base_type v_init) : v{v_init}
    {
    }

    base_type eval_at(const arg_map&) const override
    {
        return v;
    }

    std::unique_ptr<value> clone() const override
    {
        return std::make_unique<constant>(*this);
    }

private:
    base_type v;
};

class placeholder : public value
{
public:
    placeholder(std::string id_init) : id{std::move(id_init)}
    {
    }

    base_type eval_at(const arg_map& point) const override
    {
        return point.at(id);
    }

    std::unique_ptr<value> clone() const override
    {
        return std::make_unique<placeholder>(*this);
    }

private:
    std::string id;
};

using operation_t = std::function<base_type(base_type, base_type)>;

class compound : public value
{
public:
    compound(operation_t operation_init, std::unique_ptr<value> v1, std::unique_ptr<value> v2)
        : operation{operation_init}
    {
        values.emplace_back(v1->clone());
        values.emplace_back(v2->clone());
    }

    compound(const compound& other)
        : operation{other.operation}
    {
        values.reserve(other.values.size());
        std::transform(other.values.begin(), other.values.end(), std::back_inserter(values),
                       [](const auto& other_value)
                       {
                           return other_value->clone();
                       });
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
                       [&](const auto& v)
                       {
                           return v->eval_at(point);
                       });

        return std::accumulate(evaluated_values.begin() + 1, evaluated_values.end(),
                               *evaluated_values.begin(),
                               operation);
    }

    std::unique_ptr<value> clone() const override
    {
        return std::make_unique<compound>(*this);
    }

private:
    operation_t operation;
    std::vector<std::unique_ptr<value>> values;
};

class expression
{
public:
    expression(base_type c)
        : v{std::make_unique<constant>(c)}
    {
    }

    expression(std::string id)
        : v{std::make_unique<placeholder>(id)}
    {
    }

    expression(operation_t op, std::unique_ptr<value> v1, std::unique_ptr<value> v2)
        : v{std::make_unique<compound>(op, std::move(v1), std::move(v2))}
    {
    }

    base_type eval_at(const arg_map& point)
    {
        return v->eval_at(point);
    }

    std::unique_ptr<value> v;
};

expression operator+(expression e1, expression e2)
{
    return expression{std::plus<base_type>{}, std::move(e1.v), std::move(e2.v)};
}

expression operator*(expression e1, expression e2)
{
    return expression{std::multiplies<base_type>{}, std::move(e1.v), std::move(e2.v)};
}

#endif // DRAKMOOR_FUNCTION_EXPRESSION
