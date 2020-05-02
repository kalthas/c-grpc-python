from concurrent import futures
import grpc
import logging

import co_sim_pb2
import co_sim_pb2_grpc

logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(name)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)


def rm_val(request):
    logger.info(request.type)
    return request

class CoSimServicer(co_sim_pb2_grpc.CoSimServicer):

    def __init__(self):
        ""

    def RMVal(self, request, context):
        return rm_val(request)

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    co_sim_pb2_grpc.add_CoSimServicer_to_server(CoSimServicer(), server)
    server.add_insecure_port('[::]:50051')
    server.start()
    logger.info("Server started and listen on port 50051")
    server.wait_for_termination()

if __name__ == '__main__':
    serve()