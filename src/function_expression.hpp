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

class value
{
public:
    // type erasure here? or at the expression level?
    virtual ~value() = default;
    virtual base_type eval_at(const std::map<std::string, base_type>& ) = 0;
};

class constant : public value
{
public:
    constant(base_type v_init) : v{v_init}
    {
    }

    base_type eval_at(const std::map<std::string, base_type>&) override
    {
        return v;
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

    base_type eval_at(const std::map<std::string, base_type>& point) override
    {
        return point.at(id);
    }
private:
    std::string id;
};

class compound : public value
{
public:
    using operation_t = std::function<base_type(base_type, base_type)>;
    compound(operation_t operation_init, std::unique_ptr<value> v1, std::unique_ptr<value> v2)
        : operation{operation_init}, values{std::move(v1), std::move(v2)}
    {
    }


    base_type eval_at(const std::map<std::string, base_type>& point) override
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

private:
    operation_t operation;
    std::vector<std::unique_ptr<value>> values;
};

compound operator+(std::unique_ptr<value> v1, std::unique_ptr<value> v2)
{
    return compound{std::plus<base_type>{}, std::move(v1), std::move(v2)};
}

class expression
{
    // aggregate values and op?
};



#endif // DRAKMOOR_FUNCTION_EXPRESSION
