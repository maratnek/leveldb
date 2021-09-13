## Test leveldb for single thread without batches

- Test command: /home/maratworker/Sandbox/c++/leveldb/cmake-build-debug/bin/easy-load-leveldb
- Test timeout computed to be: 1500
- Prepare data
- Prepared 10000000 count
- Prepared Time: 27.0984 sec
- Write to db: 10000000 values.
- Write Time: 62.0281 sec
- Read from db: 10000000 values.
- Read Time: 72.739 sec
- Delete Time: 56.3994 sec
- Read from db: 10000000 values.
- Read Time: 68.4739 sec

## Test leveldb for multi thread without batches

- Test command: /home/maratworker/Sandbox/c++/leveldb/cmake-build-debug/bin/multi-load-leveldb
- Test timeout computed to be: 1500
- Prepare data
- Real thread count: 8
- Wait futures
- Prepared Time: 5.4463 sec
- Write data
- Real thread count: 8
- Wait futures
- Common count 10000000
- Writed Time: 69.5104 sec
- Read data
- Real thread count: 8
- Wait futures
- Common count 10000000
- Readed Time: 16.0396 sec
- Delete data
- Real thread count: 8
- Wait futures
- Common count 10000000
- Deleted Time: 75.9559 sec
- Read data
- Real thread count: 8
- Wait futures
- Common count 10000000
- Readed Time: 20.8519 sec