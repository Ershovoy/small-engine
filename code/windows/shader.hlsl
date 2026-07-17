struct Vertex_output
{
    float4 position : SV_POSITION;
    float2 texture_coordinate : TEXCOORD0;
};

Texture2D game_image : register(t0);
SamplerState game_image_sampler : register(s0);

Vertex_output vertex_shader(uint vertex_index : SV_VERTEXID)
{
    float2 positions[4] =
    {
        float2(-1.0f,  1.0f),
        float2( 1.0f,  1.0f),
        float2(-1.0f, -1.0f),
        float2( 1.0f, -1.0f),
    };

    float2 texture_coordinates[4] =
    {
        float2(0.0f, 1.0f),
        float2(1.0f, 1.0f),
        float2(0.0f, 0.0f),
        float2(1.0f, 0.0f),
    };

    Vertex_output output;
    output.position = float4(positions[vertex_index], 0.0f, 1.0f);
    output.texture_coordinate = texture_coordinates[vertex_index];
    return output;
}

float4 pixel_shader(Vertex_output input) : SV_TARGET
{
    float3 color = game_image.Sample(game_image_sampler, input.texture_coordinate).rgb;
    return float4(color, 1.0f);
}
