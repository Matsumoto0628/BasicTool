Mat4x4::~Mat4x4()
{
}

Mat4x4::Mat4x4(const DirectX::XMMATRIX& m)
    : mat(m)
{
}

Mat4x4 Mat4x4::operator*(const Mat4x4& rhs) const
{
    return Mat4x4(DirectX::XMMatrixMultiply(mat, rhs.mat));
}

Mat4x4 Mat4x4::Identity()
{
    return Mat4x4(DirectX::XMMatrixIdentity());
}

Mat4x4 Mat4x4::Scale(float x, float y, float z)
{
    return Mat4x4(DirectX::XMMatrixScaling(x, y, z));
}

Mat4x4 Mat4x4::Rotation(float x, float y, float z, float w) 
{
    DirectX::XMVECTOR quat = DirectX::XMVectorSet(x, y, z, w);
    DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationQuaternion(quat);

    return Mat4x4(rotMat);
}

Mat4x4 Mat4x4::Translation(float x, float y, float z)
{
    return Mat4x4(DirectX::XMMatrixTranslation(x, y, z));
}

Mat4x4 Mat4x4::Transpose() const
{
    return Mat4x4(DirectX::XMMatrixTranspose(mat));
}

// 引数の配列に、行列の値を入れる
void Mat4x4::ToFloat4x4(float out[4][4]) const 
{
    DirectX::XMStoreFloat4x4(
        reinterpret_cast<DirectX::XMFLOAT4X4*>(out), // reinterpret_castで無理やりキャストは危険
        mat
    ); 
}