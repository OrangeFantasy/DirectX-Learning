struct VSOut
{
    float4 color : Color;
    float4 pos : SV_Position;
};

cbuffer CBuffer
{
    row_major matrix transform;
};

VSOut main(float2 pos : Position, float4 color : Color)
{
    VSOut vsout;
    vsout.color = color;
    vsout.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), transform);
    return vsout;
}
