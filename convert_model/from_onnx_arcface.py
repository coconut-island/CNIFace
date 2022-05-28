import onnx
import tvm.relay as relay


if __name__ == "__main__":
    network = 'w600k_r50'

    model_path = './models/%s.onnx' % (network)

    image_shape = (1, 3, 112, 112)

    onnx_model = onnx.load(model_path)

    shape_dict = {"input.1": image_shape}
    mod, relay_params = relay.frontend.from_onnx(onnx_model, shape_dict)

    func = mod["main"]
    target = "llvm"

    with relay.build_config(opt_level=3):
        graph, lib, params = relay.build(func, target, params=relay_params)


    #save the relay model
    model_path = "./models/relay/"
    path_lib = ("%s%s.so" % (model_path, network))
    lib.export_library(path_lib)

    with open(("%s%s.json" % (model_path, network)), "w") as fo:
        fo.write(graph)
    with open(("%s%s.params" % (model_path, network)), "wb") as fo:
        fo.write(relay.save_param_dict(params))
