#define multiply_fixed32(multiplicand, multiplier) ((fixed32)((((int64)(multiplicand))*((int64)(multiplier))) >> 15))
#define divide_fixed32(dividend, divisor) ((fixed32)((((int64)(dividend)) << 15) / (divisor)))

#define int32_to_fixed32(value) ((fixed32)((value) << 15))
#define fixed32_to_int32(value) ((int32)((value) >> 15))

#define float32_to_fixed32(value) ((fixed32)((value) * 32768.0f))
#define fixed32_to_float32(value) (((float32)(value)) / 32768.0f)

// TODO: Probably not deterministic
#define sqrt_fixed32(value) (float32_to_fixed32(square_root(fixed32_to_float32(value))))
