import os
import onnx
import tvm.relay as relay


if __name__ == "__main__":
    model_name = "ch_PP-OCRv3_det_infer"

    models_dir_path = os.path.abspath(os.path.dirname(os.path.abspath(__file__)) + "/../models/")

    model_path =  models_dir_path + "/{}.onnx".format(model_name)

    image_shape = (1, 3, 640, 640)

    onnx_model = onnx.load(model_path)

    shape_dict = {"x": image_shape}
    mod, relay_params = relay.frontend.from_onnx(onnx_model, shape_dict)

    func = mod["main"]

    target = "llvm"

    with relay.build_config(opt_level=3):
        graph, lib, params = relay.build(func, target, params=relay_params)


    #save the relay model
    relay_model_path = models_dir_path + "/relay/"
    path_lib = relay_model_path + "{}.so".format(model_name)
    lib.export_library(path_lib)

    with open(relay_model_path + "{}.json".format(model_name), "w") as fo:
        fo.write(graph)
    with open(relay_model_path + "{}.params".format(model_name), "wb") as fo:
        fo.write(relay.save_param_dict(params))
