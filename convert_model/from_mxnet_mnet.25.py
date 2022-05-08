import mxnet as mx
import tvm.relay as relay


if __name__ == "__main__":
    network = 'mnet.25'
    device = 'x86.cpu'

    path = './models/retinaface_mnet025_v1/%s' % (network)

    image_shape = (1, 3, 640, 480)

    mx_sym, args, auxs = mx.model.load_checkpoint(path, 0)

    shape_dict = {"data": image_shape}
    mod, relay_params = relay.frontend.from_mxnet(mx_sym, shape_dict, arg_params=args, aux_params=auxs)


    func = mod["main"]
    # func = relay.Function(func.params, relay.nn.softmax(func.body), None, func.type_params, func.attrs)

    target = "llvm"
    # with tvm.transform.PassContext(opt_level=3):
    #     lib = relay.build(func, target, params=relay_params)

    with relay.build_config(opt_level=3):
        graph, lib, params = relay.build(func, target, params=relay_params)


    #save the relay model
    model_path = "./models/relay/retinaface_mnet025_v1/"
    path_lib = ("%s%s.%s.so" % (model_path, network, device))
    lib.export_library(path_lib)

    with open(("%s%s.%s.json" % (model_path, network, device)), "w") as fo:
        fo.write(graph)
    with open(("%s%s.%s.params" % (model_path, network, device)), "wb") as fo:
        fo.write(relay.save_param_dict(params))
