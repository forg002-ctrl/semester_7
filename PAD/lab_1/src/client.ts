import { io } from 'socket.io-client';
import { ClientEventTypeEnum, ServerEventTypeEnum } from './enums/eventTypes';


const HOST = '127.0.0.1';
const PORT = 3000;

const client = io(`http://${HOST}:${PORT}`);

client.on('connect', () => {
    console.log('Connected to server');

    setInterval(() => {
        client.emit(ClientEventTypeEnum.CLIENT_EVENT_1, Math.floor(Math.random() * 10));
    }, 2000);

    setTimeout(() => {
        setInterval(() => {
            client.emit(ClientEventTypeEnum.CLIENT_EVENT_2, Math.random() < 0.5);
        }, 2000);
    }, 1000);
});

client.on(ServerEventTypeEnum.SERVER_EVENT_1, (data) => {
    console.log(`Received response for event 1 from server: ${data}`);
});
  
client.on(ServerEventTypeEnum.SERVER_EVENT_2, (data) => {
    console.log(`Received response for event 2 from server: ${data}`);
});

client.on('disconnect', () => {
    console.log('Disconnected from server');
});