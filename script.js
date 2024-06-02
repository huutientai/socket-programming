document.addEventListener('DOMContentLoaded', function() {
    fetch('data.txt')
        .then(response => response.text())
        .then(data => {
            const chatWindow = document.getElementById('chat-window');
            const lines = data.split('\n');
            lines.forEach(line => {
                const messageDiv = document.createElement('div');
                messageDiv.className = 'message received';
                messageDiv.textContent = line;
                chatWindow.appendChild(messageDiv);
            });
        })
        .catch(error => console.error('Error fetching the file:', error));
});

function updateRecipientName() {
    const recipientInput = document.getElementById('recipient-input');
    const recipientName = document.getElementById('recipient-name');
    recipientName.textContent = recipientInput.value;
}

function sendMessage() {
    const messageInput = document.getElementById('message-input');
    const chatWindow = document.getElementById('chat-window');
    if (messageInput.value.trim() !== '') {
        const messageDiv = document.createElement('div');
        messageDiv.className = 'message sent';
        messageDiv.textContent = messageInput.value;
        chatWindow.appendChild(messageDiv);
        messageInput.value = '';
    }
}
