sosButton.addEventListener('click', () => {
    updateArrows(); 
    sosButton.classList.toggle('active');
    fetch('http://192.168.190.160/trigger', { 
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({ command: 'sos_trigger' })
    })
    .then(response => response.json())
    .then(data => console.log('Success:', data))
    .catch(error => console.error('Error:', error));
});
