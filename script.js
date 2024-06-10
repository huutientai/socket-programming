// Update recipient's name based on input
function updateRecipientName() {
    const recipientInput = document.getElementById('recipient-input').value;
    document.getElementById('recipient-name').textContent = recipientInput;
}

// Function to send a message using POST request
async function sendMessage() {
    const messageInput = document.getElementById('message-input').value;
    if (messageInput.trim() !== '') {
        const response = await fetch('/send', {
            method: 'POST',
            headers: {
                'Content-Type': 'text/plain'
            },
            body: messageInput
        });

        if (response.ok) {
            document.getElementById('message-input').value = '';
            await loadMessages();
        } else {
            console.error('Failed to send message');
        }
    }
}

// Function to load messages using GET request
async function loadMessages() {
    const response = await fetch('/messages');
    const messages = await response.text();
    document.getElementById('chat-box').innerHTML = messages.replace(/\n/g, '<br>');
}

// Load messages initially
document.addEventListener('DOMContentLoaded', (event) => {
    loadMessages();
});

// Periodically refresh messages every 5 seconds
setInterval(loadMessages, 5000);
