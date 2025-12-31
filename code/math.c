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

// static Vec2 vec2_add(Vec2 first_summand, second_summand)
// {
//     Vec2 sum;

//     sum.e1 = first_summand.e1 + second_summand.e1;
//     sum.e2 = first_summand.e2 + second_summand.e2;

//     return sum;
// }
