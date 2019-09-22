struct PSInput {
	float4	position	: SV_POSITION;
};


PSInput VSMain( float3 position : POSITION ) {
	PSInput	result;

	result.position = float4( position, 1.f );

	return result;
}

float4 PSMain( PSInput input ) : SV_TARGET
{
	return float4( 1.f, 1.f, 1.f, 1.f );
}