const ws = new WebSocket('ws://92.110.250.80:3000');

ws.onopen = () => {
    const status = document.querySelector('#status');
    status.textContent = 'Connected!';
    status.style.color = '#3ecc69';
}

ws.onmessage = (context) => {
    console.log(context);
}

const test = () => {
    ws.send(JSON.stringify({
        type: 1,
        data: "uwuowo",
    }));
}
