# Python
## Install requirements
```bash
cd python
pip install -r ./requirements.txt
```
## Generate python gprc/protobuf code
```bash
python -m grpc_tools.protoc -I../protos --python_out=. --grpc_python_out=. ../protos/co_sim.proto
```
## Run python grpc server
```bash
python co_sim_server.py
```
# C
## Install gRPC/C++
Follow through https://grpc.io/docs/tutorials/basic/cpp/ to build/install gRPC/C++. Pay attention to step 'Clone the grpc repo and its submodules', and make sure all the git submodules are cloned successfully.
## Build client
```bash
cd c
mkdir -p cmake/build
cd cmake/build
cmake ../..
make -j
```
## Run c grpc client
```bash
./co_sim_client
```
