struct Well
{
    float4x4 skeletonSpaceMatrix;
    float4x4 skeletonSpaceInverseTransposeMatrix;
};

struct Vertex
{
    float4 position;
    float2 texcoord;
    float3 normal;
};

struct VertexInfluence
{
    float4 weight;
    int4 index;
};

struct SkinningInformation
{
    uint numVertices;
};

StructuredBuffer<Well> gMatrixPalette : register(t0);
StructuredBuffer<Vertex> gInputVertices : register(t1);
StructuredBuffer<VertexInfluence> gInfluences : register(t2);
RWStructuredBuffer<Vertex> gOutputVertices : register(u0);
ConstantBuffer<SkinningInformation> gSkinningInformation : register(b0);

Vertex Skinning(Vertex _v, VertexInfluence _vi)
{
    Vertex skinned;
    skinned.position = mul(_v.position, gMatrixPalette[_vi.index.x].skeletonSpaceMatrix) * _vi.weight.x;
    skinned.position += mul(_v.position, gMatrixPalette[_vi.index.y].skeletonSpaceMatrix) * _vi.weight.y;
    skinned.position += mul(_v.position, gMatrixPalette[_vi.index.z].skeletonSpaceMatrix) * _vi.weight.z;
    skinned.position += mul(_v.position, gMatrixPalette[_vi.index.w].skeletonSpaceMatrix) * _vi.weight.w;
    skinned.position.w = 1.0f;
    
    // normal
    skinned.normal = mul(_v.normal, (float3x3) gMatrixPalette[_vi.index.x].skeletonSpaceInverseTransposeMatrix) * _vi.weight.x;
    skinned.normal += mul(_v.normal, (float3x3) gMatrixPalette[_vi.index.y].skeletonSpaceInverseTransposeMatrix) * _vi.weight.y;
    skinned.normal += mul(_v.normal, (float3x3) gMatrixPalette[_vi.index.z].skeletonSpaceInverseTransposeMatrix) * _vi.weight.z;
    skinned.normal += mul(_v.normal, (float3x3) gMatrixPalette[_vi.index.w].skeletonSpaceInverseTransposeMatrix) * _vi.weight.w;
    skinned.normal = normalize(skinned.normal);
    
    return skinned;
}

[numthreads(1024, 1, 1)]
void main(uint DTid : SV_DispatchThreadID)
{
    uint vertexIndex = DTid.x;
    if (vertexIndex < gSkinningInformation.numVertices)
    {
        // Get input vertex and influence
        Vertex input = gInputVertices[vertexIndex];
        VertexInfluence influence = gInfluences[vertexIndex];
        
        // Calculate skinned vertex
        Vertex skinned;
        skinned = Skinning(input, influence);
        skinned.texcoord = input.texcoord;

        // Write to buffer
        gOutputVertices[vertexIndex] = skinned;
    }
}