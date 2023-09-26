import { Server } from 'socket.io';
import http from 'http';
import { ClientEventTypeEnum, ServerEventTypeEnum } from './enums/eventTypes';

const server = http.createServer();
const io = new Server(server);

io.on('connection', (socket) => {
  console.log('Client connected');

  socket.on(ClientEventTypeEnum.CLIENT_EVENT_1, (data: number) => {
    console.log(`Received from client (event 1): ${data}`);

    if (data % 2 === 0) {
        socket.emit(ServerEventTypeEnum.SERVER_EVENT_1, `Even number`);
    } else {
        socket.emit(ServerEventTypeEnum.SERVER_EVENT_1, `Odd number`);
    }
  });

  socket.on(ClientEventTypeEnum.CLIENT_EVENT_2, (data: boolean) => {
    console.log(`Received from client (event 2): ${data}`);

    if (data) {
        socket.emit(ServerEventTypeEnum.SERVER_EVENT_2, `Heads`);
    } else {
        socket.emit(ServerEventTypeEnum.SERVER_EVENT_2, `Tails`);
    }
  });

  socket.on('end', () => {
    console.log('Client disconnected');
  });

  socket.on('error', (err) => {
    console.error(`Socket error: ${err.message}`);
  });
});

const PORT = 3000;
const HOST = '127.0.0.1';

server.listen(PORT, HOST, () => {
  console.log(`Server listening on ${HOST}:${PORT}`);
});