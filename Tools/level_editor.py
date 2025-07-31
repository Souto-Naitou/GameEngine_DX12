import bpy
import bpy_extras
import gpu
import gpu_extras.batch
import mathutils
import math
import copy
import json

# ブレンダーに登録する情報
bl_info = {
    "name": "レベルエディタ",
    "author": "Souto Naitou",
    "version": (1, 0),
    "blender": (3,3,1),
    "location": "",
    "description": "レベルエディタ",
    "warning" : "",
    "wiki_url" : "",
    "tracker_url" : "",
    "category" : "Object"
}


# URLを開くオペレータを定義した関数
def draw_menu_manual(self, context):
    # トップバーの「エディターメニュー」に項目(オペレータ)を追加
    self.layout.operator("wm.url_open_preset", text="Manual", icon="HELP")


class TOPBAR_MT_my_menu(bpy.types.Menu):
    # Blenderがクラスを識別するための固有文字列
    bl_idname = "TOPBAR_MT_my_menu"
    # メニューのラベルとして表示される文字列
    bl_label = "Nima Engine"
    # メニューの説明として表示される文字列
    bl_description = "拡張メニュー by " + bl_info["author"]

    # サブメニューの描画
    def draw(self, context):
        # トップバーの「エディターメニュー」に項目(オペレータ)を追加
        self.layout.operator(MYADDON_OT_stretch_vertex.bl_idname, text=MYADDON_OT_stretch_vertex.bl_label)
        self.layout.operator(MYADDON_OT_create_ico_sphere.bl_idname, text=MYADDON_OT_create_ico_sphere.bl_label)
        self.layout.operator(MYADDON_OT_export_scene.bl_idname, text=MYADDON_OT_export_scene.bl_label)

    # 既存のメニューにサブメニューを追加
    def submenu(self, context):
        # ID指定でサブメニューを追加
        self.layout.menu(TOPBAR_MT_my_menu.bl_idname)


class MYADDON_OT_stretch_vertex(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_stretch_vertex"
    bl_label = "頂点を伸ばす"
    bl_description = "頂点座標を引っ張って伸ばします"
    # Redo と Undo を有効化
    bl_options = {'REGISTER', 'UNDO'}

    # メニューを実行したときに呼ばれるコールバック関数
    def execute(self, context):
        bpy.data.objects["Cube"].data.vertices[0].co.x += 1.0
        print("頂点を伸ばしました。")

        # オペレータの命令終了を通知
        return {'FINISHED'}
    

class MYADDON_OT_create_ico_sphere(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_create_object"
    bl_label = "ICO球生成"
    bl_description = "ICO球を生成します"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        bpy.ops.mesh.primitive_ico_sphere_add()
        print("ICO球を生成しました。")

        return {'FINISHED'}


class MYADDON_OT_export_scene(bpy.types.Operator, bpy_extras.io_utils.ExportHelper):
    bl_idname = "myaddon.myaddon_ot_export_scene"
    bl_label = "シーン出力"
    bl_description = "シーン情報をExportします"
    filename_ext = ".json"

    def write_and_print(self, f, str):
        print(str)
        f.write(str + '\n')

    def parse_scene_recursive(self, file, object, level):
        """シーン解析用再帰関数"""

        # 深さ分インデントする (タブを挿入)
        indent = ''
        for i in range(level):
            indent += '\t'

        # オブジェクト名書き込み
        self.write_and_print(file, indent + object.type)
        trans, rot, scale = object.matrix_local.decompose()
        # 回転をQuaternionからEulerに変換
        rot = rot.to_euler()
        # 出力
        self.write_and_print(file, indent + "T %f, %f, %f" % (trans.x, trans.y, trans.z))
        self.write_and_print(file, indent + "R %f, %f, %f" % (rot.x, rot.y, rot.z))
        self.write_and_print(file, indent + "S %f, %f, %f" % (scale.x, scale.y, scale.z))
        # カスタムプロパティ 'file_name'
        if "file_name" in object:
            self.write_and_print(file, indent + "N %s" % object['file_name'])
        # カスタムプロパティ 'collision'
        if "collider" in object:
            self.write_and_print(file, indent + "C %s" % object['collider'])
            temp_str = indent + "CC %f %f %f"
            temp_str %= (object["collider_center"][0], object["collider_center"][1], object["collider_center"][2])
            self.write_and_print(file, temp_str)
            temp_str = indent + "CS %f %f %f"
            temp_str %= (object["collider_size"][0], object["collider_size"][1], object["collider_size"][2])
            self.write_and_print(file, temp_str)

        self.write_and_print(file, indent + 'END')
        self.write_and_print(file, '')


        
        # 子ノードへ進む (深さが1増える)
        for child in object.children:
            # 子ノードを再帰的に走査
            self.parse_scene_recursive(file, child, level + 1)

    def parse_scene_recursive_json(self, data_parent, object, level):
        # シーンのオブジェクト1個分のJSONオブジェクトを生成
        json_object = dict()
        # オブジェクト種類
        json_object["type"] = object.type
        # オブジェクト名
        json_object["name"] = object.name
        
        # Todo: その他情報をパック
        trans, rot, scale = object.matrix_local.decompose()
        rot = rot.to_euler()

        transform = dict()
        transform["translation"] = (trans.x, trans.y, trans.z)
        transform["rotation"] = (rot.x, rot.y, rot.z)
        transform["scales"] = (scale.x, scale.y, scale.z)
        json_object["transform"] = transform

        # カスタムプロパティ 'file_name'
        if "file_name" in object:
            json_object["file_name"] = object["file_name"]
        # カスタムプロパティ 'collider'
        if "collider" in object:
            collider = dict()
            collider["type"] = object["collider"]
            collider["center"] = object["collider_center"].to_list()
            collider["size"] = object["collider_size"].to_list()
            json_object["collider"] = collider
        # カスタムプロパティ 'uv_tiling'
        if "uv_tiling" in object:
            json_object["uv_tiling"] = object["uv_tiling"].to_list()

        # 1個分のJSONオブジェクトを親オブジェクトに登録
        data_parent.append(json_object)

        # Todo: 直接子供リストを走査
        if len(object.children) > 0:
            # 子ノードリストを作成
            json_object["children"] = list()
            
            # 子ノードへ進む (深さが１増える)
            for child in object.children:
                self.parse_scene_recursive_json(json_object["children"], child, level + 1)

            


    def export(self):
        """ファイルに出力"""

        print("シーン情報出力開始... %r" % self.filepath)

        # ファイルをテキスト形式で書き込み用にオープン
        # スコープを抜けると自動的にクローズされる <- いいね！
        with open(self.filepath, "wt") as f:
            # ファイルに文字列を書き込む
            self.write_and_print(f, "SCENE")
            self.write_and_print(f, '')
            for object in bpy.context.scene.objects:
                # 親オブジェクトがあるものはスキップ (代わりに親から呼び出すため)
                if object.parent:
                    continue

                # シーン直下のオブジェクトをルートノードとし、再帰関数で走査
                self.parse_scene_recursive(f, object, 0)

    def export_json(self):
        """JSON形式でファイルに出力"""

        # 保存する情報をまとめるdictionary
        json_object_root = dict()

        # ノード名
        json_object_root["name"] = "scene"
        # オブジェクトのリストを作成
        json_object_root["objects"] = list()

        # Todo: シーン内の全オブジェクトを走査してパック
        for object in bpy.context.scene.objects:
            # 親オブジェクトがあるものはスキップ (代わりに親から呼び出すため)
            if (object.parent):
                continue
            # シーン直下のオブジェクトをルートノードとし、再帰関数で走査
            self.parse_scene_recursive_json(json_object_root["objects"], object, 0)

        # オブジェクトをJSON文字列にエンコード
        json_text = json.dumps(
            json_object_root, 
            ensure_ascii=False, 
            cls=json.JSONEncoder,
            indent=4)
        
        # コンソールに表示
        print(json_text)

        # ファイルをテキスト形式で書き込み用にオープン
        # スコープを抜けると自動的にクローズされる
        with open(self.filepath, "wt", encoding="utf-8") as f:
            # ファイルに文字列を書き込む
            f.write(json_text)

    def execute(self, context):
        print("シーン情報をExportします。")

        # ファイルに出力
        self.export_json()

        print("シーン情報をExportしました。")
        self.report({'INFO'}, "シーン情報をExportしました。")

        # オペレータの命令終了を通知
        return {'FINISHED'}


class MYADDON_OT_add_filename(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_filename"
    bl_label = "FileName 追加"
    bl_description = "['file_name']カスタムプロパティを追加します"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        # カスタムプロパティを追加
        bpy.context.object["file_name"] = "file_name"

        return {'FINISHED'}


class MYADDON_OT_add_collider(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_collider"
    bl_label = "コライダー 追加"
    bl_description = "['collider']カスタムプロパティを追加します"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        # カスタムプロパティを追加
        context.object["collider"] = "BOX"
        context.object["collider_center"] = mathutils.Vector((0,0,0))
        context.object["collider_size"] = mathutils.Vector((2,2,2))

        return {'FINISHED'}


class MYADDON_OT_remove_collider(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_remove_collider"
    bl_label = "コライダー 削除"
    bl_description = "['collider']カスタムプロパティを削除します"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        # カスタムプロパティを削除
        if "collider" in context.object:
            del context.object["collider"]
        if "collider_center" in context.object:
            del context.object["collider_center"]
        if "collider_size" in context.object:
            del context.object["collider_size"]

        return {'FINISHED'}

class MYADDON_OT_add_uvtiling(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_uvtiling"
    bl_label = "UV Tiling 追加"
    bl_description = "['uv_tiling']カスタムプロパティを追加します"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        # カスタムプロパティを追加
        context.object["uv_tiling"] = mathutils.Vector((1.0, 1.0))

        return {'FINISHED'}
    
class MYADDON_OT_remove_uvtiling(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_remove_uvtiling"
    bl_label = "UV Tiling 削除"
    bl_description = "['uv_tiling']カスタムプロパティを削除します"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        # カスタムプロパティを削除
        if "uv_tiling" in context.object:
            del context.object["uv_tiling"]

        return {'FINISHED'}

class OBJECT_PT_file_name(bpy.types.Panel):
    """オブジェクトのファイルネームパネル"""
    bl_idname = "OBJECT_PT_file_name"
    bl_label = "FileName"
    bl_space_type = 'PROPERTIES'
    bl_region_type = "WINDOW"
    bl_context = "object"

    # サブメニューの描画
    def draw(self, context):
        # パネルに項目を追加
        if "file_name" in context.object:
            # すでにプロパティがあれば、プロパティを表示
            self.layout.prop(context.object, '["file_name"]', text=self.bl_label)
        else:
            # プロパティがなければ、プロパティを追加するオペレータを表示
            self.layout.operator(MYADDON_OT_add_filename.bl_idname)


class OBJECT_PT_collider(bpy.types.Panel):
    bl_idname = "OBJECT_PT_collider"
    bl_label = "Collider"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    def draw(self, context):
        if "collider" in context.object:
            # すでにプロパティがあれば、プロパティを表示
            self.layout.prop(context.object, '["collider"]', text="Type")
            self.layout.prop(context.object, '["collider_center"]', text="Center")
            self.layout.prop(context.object, '["collider_size"]', text="Size")
            # 削除ボタンを追加
            self.layout.operator(MYADDON_OT_remove_collider.bl_idname)
        else:
            self.layout.operator(MYADDON_OT_add_collider.bl_idname)


class MATERIAL_PT_uvtiling(bpy.types.Panel):
    bl_idname = "MATERIAL_PT_uvtiling"
    bl_label = "UV Tiling"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "material"

    def draw(self, context):
        if "uv_tiling" in context.object:
            self.layout.prop(context.object, '["uv_tiling"]', text="Tiling")
            self.layout.operator(MYADDON_OT_remove_uvtiling.bl_idname)
        else:
            self.layout.operator(MYADDON_OT_add_uvtiling.bl_idname)



class DrawCollider:
    # 描画ハンドル
    handle = None

    # 3Dビューに登録する描画関数
    def draw_collider():
        vertices = {"pos":[]}
        indices = []

        # 各頂点の、オブジェクト中心からのオフセット
        offset = 0.5
        offsets = [
            [-offset, -offset, -offset], # 左下前
            [+offset, -offset, -offset], # 右下前
            [-offset, +offset, -offset], # 左上前
            [+offset, +offset, -offset], # 右上前
            [-offset, -offset, +offset], # 左下奥
            [+offset, -offset, +offset], # 右下奥
            [-offset, +offset, +offset], # 左上奥
            [+offset, +offset, +offset], # 右上奥
        ]

        # 立方体のX, Y, Z方向サイズ
        size = [2,2,2]

        # 現在シーンのオブジェクトリストを走査
        for object in bpy.context.scene.objects:
            # コライダープロパティがなければ描画をスキップ
            if not "collider" in object:
                continue

            # 中心点、サイズの変数を宣言
            center = mathutils.Vector((0,0,0))
            size = mathutils.Vector((2,2,2))

            # プロパティから値を取得
            for i in range(3):
                center[i]=object["collider_center"][i]
                size[i]=object["collider_size"][i]

            # 追加前の頂点数
            start = len(vertices["pos"])
            # Boxの8頂点分回す
            for ofs in offsets:
                pos = center.copy()
                # 中心点を基準に各頂点ごとにずらす
                for i in range(3):
                    pos[i] += ofs[i] * size[i]
                # ローカル座標からワールド座標に変換
                pos = object.matrix_world @ pos
                # 頂点データリストに座標を追加
                vertices['pos'].append(pos)
            
            # 前面を構成する辺の頂点インデックス
            indices.append([start+0, start+1])
            indices.append([start+2, start+3])
            indices.append([start+0, start+2])
            indices.append([start+1, start+3])
            # 奥面を構成する辺の頂点インデックス
            indices.append([start+4, start+5])
            indices.append([start+6, start+7])
            indices.append([start+4, start+6])
            indices.append([start+5, start+7])
            # 前と頂点をつなぐ辺の頂点インデックス
            indices.append([start+0, start+4])
            indices.append([start+1, start+5])
            indices.append([start+2, start+6])
            indices.append([start+3, start+7])

        # Blender4.4では"3D_UNIFORM_COLOR"が使用できないため、"UNIFORM_COLOR"を使用する
        shader = gpu.shader.from_builtin("UNIFORM_COLOR")

        batch = gpu_extras.batch.batch_for_shader(shader, 'LINES', vertices, indices=indices)

        color = [0.5, 1.0, 1.0, 1.0]
        shader.bind()
        shader.uniform_float("color", color)
        # 描画
        batch.draw(shader)


# Blenderに登録するクラスリスト
classes = (
    MYADDON_OT_stretch_vertex,
    MYADDON_OT_create_ico_sphere,
    MYADDON_OT_export_scene,
    TOPBAR_MT_my_menu,
    MYADDON_OT_add_filename,
    OBJECT_PT_file_name,
    MYADDON_OT_add_collider,
    MYADDON_OT_remove_collider,
    OBJECT_PT_collider,
    MYADDON_OT_add_uvtiling,
    MYADDON_OT_remove_uvtiling,
    MATERIAL_PT_uvtiling,
)


# 有効化時のコールバック
def register():
    # クラスを登録
    for cls in classes:
        bpy.utils.register_class(cls)

    # メニューに追加
    bpy.types.TOPBAR_MT_editor_menus.append(TOPBAR_MT_my_menu.submenu)
    # 3Dビューに描画ハンドルを追加
    DrawCollider.handle = bpy.types.SpaceView3D.draw_handler_add(
            DrawCollider.draw_collider, (), 'WINDOW', 'POST_VIEW')

    print("レベルエディタが有効化されました。")


# 無効化時のコールバック
def unregister():
    # メニューから削除
    bpy.types.TOPBAR_MT_editor_menus.remove(TOPBAR_MT_my_menu.submenu)
    # 3Dビューから描画ハンドルを削除
    bpy.types.SpaceView3D.draw_handler_remove(DrawCollider.handle, 'WINDOW')

    # Unregister classes
    for cls in classes:
        bpy.utils.unregister_class(cls)

    print("レベルエディタが無効化されました。")


# main関数
if __name__ == "__main__":
    register()