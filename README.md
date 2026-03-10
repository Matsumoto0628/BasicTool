# EffectTool

パーティクルのエフェクトを作成し、スプライトシートで出力するツール
<img width="1198" height="712" alt="スクリーンショット 2026-03-10 165122" src="https://github.com/user-attachments/assets/56416fc5-1389-4679-ad12-e5c0fec13e10" />

- **開発期間** : 1ヶ月
- **制作人数** : 1人
- **リリース先** : GitHub Releases

### 動作環境

- **OS** : Windows 11
- **GPU** : DirectX 11 対応

### 参考書籍

- 『独習C++ 新版』 高橋 航平
- 『Effective C++』 Scott Meyers

<br>
<br>

## 背景

チーム制作をしていると、デザイナーやプランナーはUnityに不慣れなことが多い。

実際にUnityでエフェクトを作るには、以下の手順が必要がある。

1. リポジトリをCloneする
2. プロジェクトを開く
3. ParticleSystemで制作してPrefabにする

加えてUnityのような汎用エンジンはあらゆる用途に対応するため機能が多く、  
使い慣れるまでに時間がかかる。

そこで、**シンプルな操作でエフェクトを作成**し、 **スプライトシート形式で出力**することで  
幅広い環境で活用できるツールを作成した。  

<br>
<br>

## 使用技術

- **言語** : C++
- **IDE** : Visual Studio
- **グラフィックスAPI** : DirectX
- **UI** : ImGui
- **データ** : nlohmann/json

### 選定理由

#### ImGui

| ライブラリ | 特徴 |
|------------|------|
| **ImGui** | DirectXとの統合が容易で軽量 |
| Qt | 機能豊富だが大規模。ライセンス制約あり |
| wxWidgets | クロスプラットフォーム向けで本ツールには過剰 |

ゲーム・ツール開発向けに特化しており、DirectXへの組み込みが数行で完了する。
QtやwxWidgetsはGUIアプリ向けには強力だが、本ツールのような軽量なデバッグ・編集UIには過剰であるため、ImGuiを選定した。

---

#### nlohmann/json

| ライブラリ | 特徴 |
|------------|------|
| **nlohmann/json** | テキスト形式。人が読め、Gitでの差分管理が容易 |
| MessagePack / BSON | 高速・軽量だが人が読めず、差分が確認しにくい |
| RapidJSON | 高速だが、APIが冗長でコードが読みにくい |

保存データはバイナリ形式と迷ったが、テキスト形式であるJSONはGitで差分が見やすくバージョン管理がしやすい点を重視して選定した。
nlohmann/jsonはヘッダーだけで導入が簡単なうえ、直感的なAPIでコードの可読性も高い。

<br>
<br>

## 技術的な工夫

### オフスクリーンレンダリングによるポストエフェクトとスプライトシート出力

オフスクリーンにレンダリングし、ブラーのシェーダーを適用することでBloomを実装しています。

https://github.com/Matsumoto0628/BasicTool/blob/c1f91ebc48b1369e9637a90cabb5038f12fd2d51/basic_tool/scripts/application/render_context.cpp#L187-L190

1フレームずつオフスクリーンにレンダリングし、全フレームを1枚のPNGとして並べて出力する。

https://github.com/Matsumoto0628/BasicTool/blob/8ab9197dc6c06e20f99159328d0ff0701fbc7823/basic_tool/scripts/application/render_exporter.cpp#L103-L112

---

### コンポーネント指向 : テンプレートによるジェネリック実装

`ComponentHolder` にテンプレートを用いることで、型ごとにコードを書かずにあらゆるコンポーネントを追加・取得できる。

https://github.com/user/repo/blob/commit_hash/file.cpp#L10-L30

---

### JSON によるセーブ & ロード

各コンポーネントが `Save` / `Load` を実装することで、`ComponentHolder` 側は型を意識せず一括で保存・復元できる。

https://github.com/user/repo/blob/commit_hash/file.cpp#L10-L30

<br>
<br>

## デモ

<!-- デモ動画をここに挿入 -->
<!-- [![デモ動画](images/thumbnail.png)](動画URL) -->

<br>
<br>

## インストール方法

<!-- スクリーンショットをここに挿入 -->
1. GitHubのReleasesページから `EffectTool.zip` をダウンロード
2. ZIPを展開する
3. `EffectTool.exe` を起動する

> **Note:** 起動時にWindowsのセキュリティ警告が表示される場合があります。「詳細情報」>「実行」をクリックして続行してください。
>

<br>
<br>

## 操作方法

### エフェクトの編集

<!-- スクリーンショットをここに挿入 -->
1. 左側の **Hierarchy** パネルで `ParticleController` をクリック
2. 右側の **Inspector** パネルに `ParticleController` コンポーネントのプロパティが表示される
3. `Gravity` や `Color` などのパラメータを変更するとパーティクルがリアルタイムで変化する

### 画像書き出し

<!-- スクリーンショットをここに挿入 -->
- **書き出し** : File > Export (出力先は、exeと同じ階層の `export/` フォルダ (固定))

### 保存 & 読み込み

<!-- スクリーンショットをここに挿入 -->
- **保存** : File > Save
- **読み込み** : File > Open
