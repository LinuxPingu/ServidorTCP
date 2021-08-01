using System;
using System.Net.Sockets;
using System.Text;
using System.Windows.Forms;

namespace ClientSideChatUI
{
    public partial class Form1 : Form
    {
        TcpClient _client;

        byte[] _buffer = new byte[4096];
        public Form1()
        {
            InitializeComponent();

            _client = new TcpClient();
            _client.Connect(" ", 54000);
            _client.GetStream().BeginRead(_buffer, 0,
                                          _buffer.Length,
                                          Server_MessageReceived,
                                          null);
        }

        private void Server_MessageReceived(IAsyncResult ar)
        {
            if (ar.IsCompleted)
            {
                // End the stream read
                var bytesIn = _client.GetStream().EndRead(ar);
                if (bytesIn > 0)
                {
                    // Create a string from the received data. For this server 
                    // our data is in the form of a simple string, but it could be
                    // binary data or a JSON object. Payload is your choice.
                    var tmp = new byte[bytesIn];
                    Array.Copy(_buffer, 0, tmp, 0, bytesIn);
                    var str = Encoding.ASCII.GetString(tmp);

                    // Any actions that involve interacting with the UI must be done
                    // on the main thread. This method is being called on a worker
                    // thread so using the form's BeginInvoke() method is vital to
                    // ensure that the action is performed on the main thread.
                    BeginInvoke((Action)(() =>
                    {
                        listBox1.Items.Add(str);
                        listBox1.SelectedIndex = listBox1.Items.Count - 1;
                    }));
                }

                // Clear the buffer and start listening again
                Array.Clear(_buffer, 0, _buffer.Length);
                _client.GetStream().BeginRead(_buffer, 0,
                                              _buffer.Length,
                                              Server_MessageReceived,
                                              null);
            }
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void Enviar_Click(object sender, EventArgs e)
        {
            // Encode the message and send it out to the server.
            var msg = Encoding.ASCII.GetBytes(textBox1.Text);
            _client.GetStream().Write(msg, 0, msg.Length);

            // Clear the text box and set it's focus
            textBox1.Text = "";
            textBox1.Focus();
        }
    }
}
