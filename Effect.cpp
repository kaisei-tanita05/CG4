#include "Effect.h"
#include <3d\Camera.h>
#include <base\DirectXCommon.h>
#include <3d\Material.h>
#include <math\MathUtility.h>
#include <base\StringUtility.h>
#include <base\TextureManager.h>
#include <3d\WorldTransform.h>
#include <algorithm>
#include <cassert>
#include <d3dcompiler.h>
#include <format>
#include <fstream>
#include <numbers>
#include <sstream>
#include "Effect.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace std;
using namespace Microsoft::WRL;

namespace KamataEngine {
	const char* Effect::kBaseDirectory = "Resources/";
	const char* Effect::kDefaultModelName = "cube";
	EffectCommon* EffectCommon::sInstance_ = nullptr;
	void Effect::StaticInitialize() { EffectCommon::GetInstance()->Initialize(); }
	void Effect::StaticFinalize() { EffectCommon::GetInstance()->Terminate(); }
	Effect* Effect::Create() {
		// メモリ確保
		Effect* instance = new Effect;
		instance->InitializeFromFile(kDefaultModelName, false);
		return instance;
	}
	Effect* Effect::CreateFromOBJ(const std::string& modelname, bool smoothing) {
		// メモリ確保
		Effect* instance = new Effect;
		instance->InitializeFromFile(modelname, smoothing);
		return instance;
	}

	Effect* Effect::CreateSphere(uint32_t divisionVertial, uint32_t divisionHorizontal) {
	    // メモリ確保
	    Effect* instance = new Effect;
	    std::vector<Mesh::VertexPosNormalUv> vertices;
	    std::vector<uint32_t> indices;
	    // 頂点数
	    const uint32_t kNumSphereVertices = divisionVertial * divisionHorizontal * 4;
	    // インデックス数
	    const uint32_t kNumSphereIndices = divisionVertial * divisionHorizontal * 6;

	    vertices.resize(kNumSphereVertices);
	    indices.resize(kNumSphereIndices);

	    float pi = std::numbers::pi_v<float>;

	    // 経度分割1つ分の角度
	    const float kLonEvery = pi * 2.0f / float(divisionHorizontal);
	    // 緯度分割1つ分の角度
	    const float kLatEvery = pi / float(divisionVertial);

	    // 座標計算
	    // 緯度の方向に分割
	    for (uint32_t latIndex = 0; latIndex < divisionVertial; ++latIndex) {
		    float lat = -pi / 2.0f + kLatEvery * latIndex;
		    // 経度の方向に分割しながら線を描く
		    for (uint32_t lonIndex = 0; lonIndex < divisionHorizontal; ++lonIndex) {
			    uint32_t startIndex = (latIndex * divisionHorizontal + lonIndex) * 4;
			    float lon = lonIndex * kLonEvery;
			    // 左下
			    vertices[startIndex].pos.x = std::cos(lat) * std::cos(lon);
			    vertices[startIndex].pos.y = std::sin(lat);
			    vertices[startIndex].pos.z = std::cos(lat) * std::sin(lon);
			    vertices[startIndex].uv = {float(lonIndex) / float(divisionHorizontal), 1.0f - float(latIndex) / float(divisionVertial)};
			    vertices[startIndex].normal = vertices[startIndex].pos;
			    vertices[startIndex].normal = MathUtility::Normalize(vertices[startIndex].normal);
			    // 左上
			    vertices[startIndex + 1].pos.x = std::cos(lat + kLatEvery) * std::cos(lon);
			    vertices[startIndex + 1].pos.y = std::sin(lat + kLatEvery);
			    vertices[startIndex + 1].pos.z = std::cos(lat + kLatEvery) * std::sin(lon);
			    vertices[startIndex + 1].uv = {float(lonIndex) / float(divisionHorizontal), 1.0f - float(latIndex + 1) / float(divisionVertial)};
			    vertices[startIndex + 1].normal = vertices[startIndex + 1].pos;
			    vertices[startIndex + 1].normal = MathUtility::Normalize(vertices[startIndex + 1].normal);
			    // 右下
			    vertices[startIndex + 2].pos.x = std::cos(lat) * std::cos(lon + kLonEvery);
			    vertices[startIndex + 2].pos.y = std::sin(lat);
			    vertices[startIndex + 2].pos.z = std::cos(lat) * std::sin(lon + kLonEvery);
			    vertices[startIndex + 2].uv = {float(lonIndex + 1) / float(divisionHorizontal), 1.0f - float(latIndex) / float(divisionVertial)};
			    vertices[startIndex + 2].normal = vertices[startIndex + 2].pos;
			    vertices[startIndex + 2].normal = MathUtility::Normalize(vertices[startIndex + 2].normal);
			    // 右上
			    vertices[startIndex + 3].pos.x = std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);
			    vertices[startIndex + 3].pos.y = std::sin(lat + kLatEvery);
			    vertices[startIndex + 3].pos.z = std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);
			    vertices[startIndex + 3].uv = {float(lonIndex + 1) / float(divisionHorizontal), 1.0f - float(latIndex + 1) / float(divisionVertial)};
			    vertices[startIndex + 3].normal = vertices[startIndex + 3].pos;
			    vertices[startIndex + 3].normal = MathUtility::Normalize(vertices[startIndex + 3].normal);
		    }
	    }

	    // インデックス計算
	    // 緯度の方向に分割
	    for (uint32_t latIndex = 0; latIndex < divisionVertial; ++latIndex) {
		    // 経度の方向に分割しながら線を描く
		    for (uint32_t lonIndex = 0; lonIndex < divisionHorizontal; ++lonIndex) {
			    uint32_t startIndex = (latIndex * divisionHorizontal + lonIndex) * 6;
			    uint32_t startIndexVertices = (latIndex * divisionHorizontal + lonIndex) * 4;

			    indices[startIndex + 0] = startIndexVertices + 0;
			    indices[startIndex + 1] = startIndexVertices + 1;
			    indices[startIndex + 2] = startIndexVertices + 2;
			    indices[startIndex + 3] = startIndexVertices + 2;
			    indices[startIndex + 4] = startIndexVertices + 1;
			    indices[startIndex + 5] = startIndexVertices + 3;
		    }
	    }

	    instance->InitializeFromVertices(vertices, indices);

	    return instance;
    }

	//四角形モデルを作成
	Effect* Effect::CreateSquare() { 
		//メモリ確保
		Effect* instance = new Effect;
	    std::vector<Mesh::VertexPosNormalUv> vertices;

		std::vector<uint32_t> indices;

	    // 頂点数
	    const uint32_t kNumVertices = 4;
	    // インデックス数
	    const uint32_t kNumIndices = 6;

	    vertices.resize(kNumVertices);
	    indices.resize(kNumIndices);

	    // 左下
	    vertices[0].pos = {-1.0f, -1.0f, 0.0f};
	    vertices[0].uv = {0.0f, 1.0f};
	    vertices[0].normal = {0.0f, 0.0f, -1.0f};
	    // 左上
	    vertices[1].pos = {-1.0f, 1.0f, 0.0f};
	    vertices[1].uv = {0.0f, 0.0f};
	    vertices[1].normal = {0.0f, 0.0f, -1.0f};
	    // 右下
	    vertices[2].pos = {1.0f, -1.0f, 0.0f};
	    vertices[2].uv = {1.0f, 1.0f};
	    vertices[2].normal = {0.0f, 0.0f, -1.0f};
	    // 右上
	    vertices[3].pos = {1.0f, 1.0f, 0.0f};
	    vertices[3].uv = {1.0f, 0.0f};
	    vertices[3].normal = {0.0f, 0.0f, -1.0f};

	    indices[0] = 0;
	    indices[1] = 1;
	    indices[2] = 2;
	    indices[3] = 1;
	    indices[4] = 3;
	    indices[5] = 2;

	    instance->InitializeFromVertices(vertices, indices);

	    return instance;
	}

	void Effect::PreDraw(ID3D12GraphicsCommandList* commandList) { EffectCommon::GetInstance()->PreDraw(commandList); }

	void Effect::PostDraw() { EffectCommon::GetInstance()->PostDraw(); }

	void Effect::InitializeFromFile(const std::string& modelname, bool smoothing) {
	    // モデル読み込み
	    LoadModel(modelname, smoothing);

	    // メッシュのマテリアルチェック
	    for (auto& m : meshes_) {
		    // マテリアルの割り当てがない
		    if (m->GetMaterial() == nullptr) {
			    if (defaultMaterial_ == nullptr) {
				    // デフォルトマテリアルを生成
				    defaultMaterial_ = Material::Create();
				    defaultMaterial_->name = "no material";
				    defaultMaterial_->Update();
			    }
			    // デフォルトマテリアルをセット
			    m->SetMaterial(defaultMaterial_.get());
		    }
	    }

	    // メッシュのバッファ生成
	    for (auto& m : meshes_) {
		    m->CreateBuffers();
	    }

	    // マテリアルの数値を定数バッファに反映
	    for (auto& m : materials_) {
		    m.second->Update();
	    }

	    // テクスチャの読み込み
	    LoadTextures();
	}

	void Effect::InitializeFromVertices(const std::vector<Mesh::VertexPosNormalUv>& vertices, const std::vector<uint32_t>& indices) {
	    // メッシュ生成
	    meshes_.emplace_back(std::make_unique<Mesh>());
	    Mesh* mesh = meshes_.back().get();

	    // メッシュにデータを流し込む
	    for (const auto& vertex : vertices) {
		    mesh->AddVertex(vertex);
	    }
	    for (const auto index : indices) {
		    mesh->AddIndex(index);
	    }

	    // デフォルトマテリアルを生成
	    defaultMaterial_ = Material::Create();
	    defaultMaterial_->name = "no material";
	    defaultMaterial_->Update();
	    // デフォルトマテリアルをセット
	    mesh->SetMaterial(defaultMaterial_.get());

	    // メッシュのバッファ生成
	    for (auto& m : meshes_) {
		    m->CreateBuffers();
	    }

	    // テクスチャの読み込み
	    LoadTextures();
	}

	void Effect::LoadModel(const std::string& modelname, bool smoothing) {
	    const string modelFileName = modelname + ".obj";
	    const string directoryPath = kBaseDirectory + modelname + "/";

	    // ファイルストリーム
	    std::ifstream file;
	    // .objファイルを開く
	    file.open(directoryPath + modelFileName);
	    // ファイルオープン失敗をチェック
	    if (file.fail()) {
		    auto message = std::format(
		        L"モデルデータファイル「{0}」"
		        "の読み込みに失敗しました。\n指定したパスが正しいか、必須リソースのコピー"
		        "を忘れていないか確認してください。",
		        ConvertStringMultiByteToWide(modelname));
		    MessageBoxW(nullptr, message.c_str(), L"Not found .obj", 0);
		    assert(false);
		    exit(1);
	    }

	    name_ = modelname;

	    // メッシュ生成
	    meshes_.emplace_back(std::make_unique<Mesh>());
	    Mesh* mesh = meshes_.back().get();
	    uint16_t indexCountTex = 0;

	    vector<Vector3> positions; // 頂点座標
	    vector<Vector3> normals;   // 法線ベクトル
	    vector<Vector2> texcoords; // テクスチャUV
	    // 1行ずつ読み込む
	    string line;
	    while (getline(file, line)) {

		    // 1行分の文字列をストリームに変換して解析しやすくする
		    std::istringstream line_stream(line);

		    // 半角スペース区切りで行の先頭文字列を取得
		    string key;
		    getline(line_stream, key, ' ');

		    // マテリアル
		    if (key == "mtllib") {
			    // マテリアルのファイル名読み込み
			    string materialFileName;
			    line_stream >> materialFileName;
			    // マテリアル読み込み
			    LoadMaterial(directoryPath, materialFileName);
		    }
		    // 先頭文字列がgならグループの開始
		    if (key == "g") {

			    // カレントメッシュの情報が揃っているなら
			    if (mesh->GetName().size() > 0 && mesh->GetVertexCount() > 0) {
				    // 頂点法線の平均によるエッジの平滑化
				    if (smoothing) {
					    mesh->CalculateSmoothedVertexNormals();
				    }
				    // 次のメッシュ生成
				    meshes_.emplace_back(std::make_unique<Mesh>());
				    mesh = meshes_.back().get();
				    indexCountTex = 0;
			    }

			    // グループ名読み込み
			    string groupName;
			    line_stream >> groupName;

			    // メッシュに名前をセット
			    mesh->SetName(groupName);
		    }
		    // 先頭文字列がvなら頂点座標
		    if (key == "v") {
			    // X,Y,Z座標読み込み
			    Vector3 position{};
			    line_stream >> position.x;
			    line_stream >> position.y;
			    line_stream >> position.z;
			    // X反転により、右手系のモデルデータを左手系に変換
			    position.x = -position.x;
			    positions.emplace_back(position);
		    }
		    // 先頭文字列がvtならテクスチャ
		    if (key == "vt") {
			    // U,V成分読み込み
			    Vector2 texcoord{};
			    line_stream >> texcoord.x;
			    line_stream >> texcoord.y;
			    // V方向反転
			    texcoord.y = 1.0f - texcoord.y;
			    // テクスチャ座標データに追加
			    texcoords.emplace_back(texcoord);
		    }
		    // 先頭文字列がvnなら法線ベクトル
		    if (key == "vn") {
			    // X,Y,Z成分読み込み
			    Vector3 normal{};
			    line_stream >> normal.x;
			    line_stream >> normal.y;
			    line_stream >> normal.z;
			    // 法線ベクトルデータに追加
			    normals.emplace_back(normal);
		    }
		    // 先頭文字列がusemtlならマテリアルを割り当てる
		    if (key == "usemtl") {
			    if (mesh->GetMaterial() == nullptr) {
				    // マテリアルの名読み込み
				    string materialName;
				    line_stream >> materialName;

				    // マテリアル名で検索し、マテリアルを割り当てる
				    auto itr = materials_.find(materialName);
				    if (itr != materials_.end()) {
					    mesh->SetMaterial(itr->second.get());
				    }
			    }
		    }
		    // 先頭文字列がfならポリゴン（三角形）
		    if (key == "f") {
			    int faceIndexCount = 0;
			    // 半角スペース区切りで行の続きを読み込む
			    string index_string;
			    std::array<uint16_t, 4> tempIndices;
			    while (getline(line_stream, index_string, ' ')) {
				    // 頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				    std::istringstream index_stream(index_string);
				    uint32_t indexPosition, indexNormal, indexTexcoord;
				    // 頂点番号
				    index_stream >> indexPosition;

				    Material* material = mesh->GetMaterial();
				    index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
				    // マテリアル、テクスチャがある場合
				    if (material && material->textureFilename_.size() > 0) {
					    index_stream >> indexTexcoord;
					    index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
					    index_stream >> indexNormal;
					    // 頂点データの追加
					    Mesh::VertexPosNormalUv vertex{};
					    vertex.pos = positions[indexPosition - 1];
					    vertex.normal = normals[indexNormal - 1];
					    vertex.uv = texcoords[indexTexcoord - 1];
					    mesh->AddVertex(vertex);
					    // エッジ平滑化用のデータを追加
					    if (smoothing) {
						    mesh->AddSmoothData(indexPosition, (uint32_t)mesh->GetVertexCount() - 1);
					    }
				    } else {
					    char c;
					    index_stream >> c;
					    // スラッシュ2連続の場合、頂点番号のみ
					    if (c == '/') {
						    // 頂点データの追加
						    Mesh::VertexPosNormalUv vertex{};
						    vertex.pos = positions[indexPosition - 1];
						    vertex.normal = {0, 0, 1};
						    vertex.uv = {0, 0};
						    mesh->AddVertex(vertex);
					    } else {
						    index_stream.seekg(-1, ios_base::cur); // 1文字戻る
						    index_stream >> indexTexcoord;
						    index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
						    index_stream >> indexNormal;
						    // 頂点データの追加
						    Mesh::VertexPosNormalUv vertex{};
						    vertex.pos = positions[indexPosition - 1];
						    vertex.normal = normals[indexNormal - 1];
						    vertex.uv = {0, 0};
						    mesh->AddVertex(vertex);
						    // エッジ平滑化用のデータを追加
						    if (smoothing) {
							    mesh->AddSmoothData(indexPosition, (uint32_t)mesh->GetVertexCount() - 1);
					    }
					    }
				    }

				    assert(faceIndexCount < 4 && "5角形ポリゴン以上は非対応です");

				    // インデックスデータの追加
				    tempIndices[faceIndexCount] = indexCountTex;

				    indexCountTex++;
				    faceIndexCount++;
			    }

			    // インデックスデータの順序変更で時計回り→反時計回り変換
			    mesh->AddIndex(tempIndices[0]);
			    mesh->AddIndex(tempIndices[2]);
			    mesh->AddIndex(tempIndices[1]);
			    // 四角形なら三角形を追加
			    if (faceIndexCount == 4) {
				    mesh->AddIndex(tempIndices[0]);
				    mesh->AddIndex(tempIndices[3]);
				    mesh->AddIndex(tempIndices[2]);
			    }
		    }
	    }
	    file.close();

	    // 頂点法線の平均によるエッジの平滑化
	    if (smoothing) {
		    mesh->CalculateSmoothedVertexNormals();
	    }
	}

	void Effect::LoadMaterial(const std::string& directoryPath, const std::string& filename) {
	    // ファイルストリーム
	    std::ifstream file;
	    // マテリアルファイルを開く
	    file.open(directoryPath + filename);
	    // ファイルオープン失敗をチェック
	    if (file.fail()) {
		    auto message = std::format(
		        L"マテリアルデータファイル「{0}」"
		        "の読み込みに失敗しました。\n指定したパスが正しいか、必須リソースのコピー"
		        "を忘れていないか確認してください。",
		        ConvertStringMultiByteToWide(filename));
		    MessageBoxW(nullptr, message.c_str(), L"Not found .mtl", 0);
		    assert(false);
		    exit(1);
	    }

	    std::unique_ptr<Material> material;

	    // 1行ずつ読み込む
	    string line;
	    while (getline(file, line)) {

		    // 1行分の文字列をストリームに変換して解析しやすくする
		    std::istringstream line_stream(line);

		    // 半角スペース区切りで行の先頭文字列を取得
		    string key;
		    getline(line_stream, key, ' ');

		    // 先頭のタブ文字は無視する
		    if (key[0] == '\t') {
			    key.erase(key.begin()); // 先頭の文字を削除
		    }

		    // 先頭文字列がnewmtlならマテリアル名
		    if (key == "newmtl") {

			    // 既にマテリアルがあれば
			    if (material) {
				    // マテリアルをコンテナに登録
				    AddMaterial(material);
			    }

			    // 新しいマテリアルを生成
			    material = Material::Create();
			    // マテリアル名読み込み
			    line_stream >> material->name;
		    }
		    // 先頭文字列がKaならアンビエント色
		    if (key == "Ka") {
			    line_stream >> material->ambient_.x;
			    line_stream >> material->ambient_.y;
			    line_stream >> material->ambient_.z;
		    }
		    // 先頭文字列がKdならディフューズ色
		    if (key == "Kd") {
			    line_stream >> material->diffuse_.x;
			    line_stream >> material->diffuse_.y;
			    line_stream >> material->diffuse_.z;
		    }
		    // 先頭文字列がKsならスペキュラー色
		    if (key == "Ks") {
			    line_stream >> material->specular_.x;
			    line_stream >> material->specular_.y;
			    line_stream >> material->specular_.z;
		    }
		    // 先頭文字列がmap_Kdならテクスチャファイル名
		    if (key == "map_Kd") {

			    Vector3 uvw = {1, 1, 1};
			    Vector3 offset = {0, 0, 0};

			    std::string input;
			    do {
				    line_stream >> input;

				    // スケール
				    if (input == "-s") {
					    line_stream >> material->uvScale_.x;
					    line_stream >> material->uvScale_.y;
					    line_stream >> material->uvScale_.z;
				    }
				    // 原点オフセット
				    else if (input == "-o") {
					    line_stream >> material->uvOffset_.x;
					    line_stream >> material->uvOffset_.y;
					    line_stream >> material->uvOffset_.z;
				    } else {
					    // テクスチャのファイル名読み込み
					    material->textureFilename_ = input;
					    break;
				    }
			    } while (true);

			    // フルパスからファイル名を取り出す
			    size_t pos1;
			    pos1 = material->textureFilename_.rfind('\\');
			    if (pos1 != string::npos) {
				    material->textureFilename_ = material->textureFilename_.substr(pos1 + 1, material->textureFilename_.size() - pos1 - 1);
			    }

			    pos1 = material->textureFilename_.rfind('/');
			    if (pos1 != string::npos) {
				    material->textureFilename_ = material->textureFilename_.substr(pos1 + 1, material->textureFilename_.size() - pos1 - 1);
			    }
		    }
	    }
	    // ファイルを閉じる
	    file.close();

	    if (material) {
		    // マテリアルを登録
		    AddMaterial(material);
	    }
	}

	void Effect::AddMaterial(std::unique_ptr<Material>& material) {
	    // コンテナに登録
	    materials_.emplace(material->name, std::move(material));
	}


	void Effect::LoadTextures() {
	    int textureIndex = 0;
	    string directoryPath = name_ + "/";

	    for (auto& m : materials_) {
		    std::unique_ptr<Material>& material = m.second;

		    // テクスチャあり
		    if (material->textureFilename_.size() > 0) {
			    // マテリアルにテクスチャ読み込み
			    material->LoadTexture(directoryPath);
			    textureIndex++;
		    }
		    // テクスチャなし
		    else {
			    // マテリアルにテクスチャ読み込み
			    material->LoadTexture("");
			    textureIndex++;
		    }
	    }
	}

	void Effect::Draw(const WorldTransform& worldTransform, const Camera& camera, const ObjectColor* objectColor) { 
		EffectCommon* common = EffectCommon::GetInstance();

		// ライトコマンドを積む
	    common->LightCommand(lightGroup_);

	    // トランスフォームコマンドを積む（参照からポインタへ）
	    common->TransformCommand(&worldTransform, &camera);

	    // オブジェクトアルファのコマンドを積む
	    const ObjectColor* useObjectColor = common->GetObjectColor();
	    if (objectColor) {
		    useObjectColor = objectColor;
	    }
	    useObjectColor->SetGraphicsCommand(common->GetCommandList(), (UINT)RoomParameter::kObjectColor);

	    // 全メッシュを描画
	    for (auto& mesh : meshes_) {
		    mesh->Draw(common->GetCommandList(), (UINT)RoomParameter::kMaterial, (UINT)RoomParameter::kTexture);
	    }
	}

    } // namespace KamataEngine