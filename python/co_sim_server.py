from concurrent import futures
import grpc
import logging

import co_sim_pb2
import co_sim_pb2_grpc

logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)


def rm_val(request):
    logger.info('Request received:')
    logger.info('\ttype={}'.format(request.type))
    logger.info('\tchannels={}'.format(request.channels))
    logger.info('\t# of buffer={}'.format(len(request.buffer)))
    valid = request.type != 'typex'
    request.buffer.extend([-1])
    resp = co_sim_pb2.SimResponse(
            id=request.id,
            type=request.type,
            valid=valid,
            buffer=request.buffer)
    return resp


class CoSimServicer(co_sim_pb2_grpc.CoSimServicer):

    def __init__(self):
        ""

    def RMVal(self, request, context):
        return rm_val(request)

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10),
                         options=[
                              ('grpc.max_send_message_length', 500 * 1024 * 1024),
                              ('grpc.max_receive_message_length', 500 * 1024 * 1024)
                         ])
    co_sim_pb2_grpc.add_CoSimServicer_to_server(CoSimServicer(), server)
    server.add_insecure_port('[::]:50051')
    server.start()
    logger.info("Server started and listen on port 50051")
    server.wait_for_termination()

if __name__ == '__main__':
    serve()
