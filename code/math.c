#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) < (b)) ? (b) : (a))
#define square(a) ((a) * (a))

int32 round_float32_to_int32(float32 number)
{
    if (number > 0.0f)
    {
        number += 0.5f;
    }
    else
    {
        number -= 0.5f;
    }

    return (int32)number;
}

// https://c-for-dummies.com/blog/?p=4250
static float32 square_root(float32 x)
{
    float32 y;
    int32 p, square, c;

    /* find the surrounding perfect squares */
    p = 0;
    do
    {
        p++;
        square = (p + 1) * (p + 1);
    } while (x > square);

    /* process the root */
    y = (float32)p;
    c = 0;
    while (c < 10)
    {
        /* divide and average */
        y = (x / y + y) / 2;

        /* test for success */
        if (y * y == x)
        {
            return y;
        }
        c++;
    }

    return y;
}

static Vec2 vec2_add(Vec2 first_summand, Vec2 second_summand)
{
    Vec2 sum;

    sum.e1 = first_summand.e1 + second_summand.e1;
    sum.e2 = first_summand.e2 + second_summand.e2;

    return sum;
}

static Vec2 vec2_subtract(Vec2 minuend, Vec2 subtrahend)
{
    Vec2 difference;

    difference.e1 = minuend.e1 - subtrahend.e1;
    difference.e2 = minuend.e2 - subtrahend.e2;

    return difference;
}

static Vec2 vec2_negate(Vec2 vector)
{
    Vec2 opposite_vector;

    opposite_vector.e1 = -vector.e1;
    opposite_vector.e2 = -vector.e2;

    return opposite_vector;
}

static Vec2 vec2_scale(Vec2 vector, float32 scalar)
{
    Vec2 result;

    result.e1 = vector.e1 * scalar;
    result.e2 = vector.e2 * scalar;

    return result;
}

static Vec2 vec2_multiply_by_vec2(Vec2 multiplicand_vector, Vec2 multiplier_vector)
{
    Vec2 product;

    product.e1 = multiplicand_vector.e1 * multiplier_vector.e1;
    product.e2 = multiplicand_vector.e2 * multiplier_vector.e2;

    return product;
}

static Vec2 vec2_divide_by_float32(Vec2 vector, float32 divisor)
{
    Vec2 result;

    result.e1 = vector.e1 / divisor;
    result.e2 = vector.e2 / divisor;

    return result;
}

static Vec2 vec2_divide_by_vec2(Vec2 dividend_vector, Vec2 divisor_vector)
{
    Vec2 quotient;

    quotient.e1 = dividend_vector.e1;
    quotient.e2 = dividend_vector.e2;

    if (divisor_vector.e1 != 0.0f && divisor_vector.e2 != 0.0f)
    {
        quotient.e1 /= divisor_vector.e1;
        quotient.e2 /= divisor_vector.e2;
    }

    return quotient;
}

static float32 vec2_length_squared(Vec2 vector)
{
    float32 result = vector.e1 * vector.e1 + vector.e2 * vector.e2;

    return result;
}

static float32 vec2_length(Vec2 vector)
{
    float32 result = square_root(vector.e1 * vector.e1 + vector.e2 * vector.e2);

    return result;
}

static Vec2 vec2_normalize(Vec2 vector)
{
    float32 length = vec2_length(vector);

    Vec2 normalized;

    normalized.e1 = vector.e1 / length;
    normalized.e2 = vector.e2 / length;

    return normalized;
}

static float32 vec2_dot_product(Vec2 first_vector, Vec2 second_vector)
{
    return first_vector.e1 * second_vector.e1 + first_vector.e2 * second_vector.e2;
}
