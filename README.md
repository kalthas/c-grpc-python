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