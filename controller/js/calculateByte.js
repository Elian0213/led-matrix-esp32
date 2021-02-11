const showBytes = () => {
  let byteArray = [];
  let bitValue = [128, 64, 32, 16, 8, 4, 2, 1];

  for (let row = 1; row <= 8; row++) {
    let byteValue = 0;

    for (let column = 1; column <= 8; column++) {
      const btn = document.querySelector(`#led-${row}_${column}`);
      let isChecked = btn.classList.contains('active');

      if (isChecked) {
        byteValue += bitValue[column - 1];
      }
    }

    // Convert to hexadecimal.
    byteArray.push(byteValue);
  }

  return byteArray
}

// Render board
const board = document.querySelector('.led-board');

const createButton = (id) => {
  const button = document.createElement('button');

  // Font-Awesome icon
  button.innerHTML = '<i class="fas fa-circle"></i>';
  button.id = id;

  return button;
}

for (let x = 1; x <= 8; x++) {
  for (let y = 1; y <= 8; y++) {
    const btn = createButton(`led-${x}_${y}`);

    btn.addEventListener('click', (e) => {
      if (btn.className === '') {
        btn.className = 'active';
      } else {
        btn.className = '';
      }
      update();
    })

    board.appendChild(btn);
  }
}

const update = () => {
  ws.send(JSON.stringify({
    type: 1,
    data: showBytes(),
  }));
}

document.querySelector('.reset-btn').addEventListener('click', () => {
  document.querySelectorAll('.led-board button')
  .forEach(btn => {
    btn.className = '';
  });

  update();
})

document.querySelector('.led-code').addEventListener('click', () => {
  alert(showBytes());
})
