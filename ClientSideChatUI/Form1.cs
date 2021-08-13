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
            _client.Connect("127.0.0.1", 54000);
            _client.GetStream().BeginRead(_buffer, 0,
                                          _buffer.Length,
                                          Server_MessageReceived,
                                          null);
        }

        private void Server_MessageReceived(IAsyncResult ar)
        {
            if (ar.IsCompleted)
            {
                // Finalizar la lectura del flujo
                var bytesIn = _client.GetStream().EndRead(ar);
                if (bytesIn > 0)
                {
                    // Crea un string a partir de los datos recibidos. Para este servidor 
                    //los datos están en forma de un simple string, pero podría ser datos
                    // binarios o un objeto JSON. Payload is your choice.
                    var tmp = new byte[bytesIn];
                    Array.Copy(_buffer, 0, tmp, 0, bytesIn);
                    var str = Encoding.ASCII.GetString(tmp);

                    // Cualquier acción que implique interactuar con la UI (interfaz de usuario)
                    // debe hacerse en el hilo principal. Este método es llamado en
                    //un hilo de trabajo , así que usar el método BeginInvoke() del 
                    //formulario es vital para asegurar que la acción se realiza en el hilo principal.
                    if (IsHandleCreated)
                    {
                        BeginInvoke((Action)(() =>
                        {
                            listBox1.Items.Add(str);
                            listBox1.SelectedIndex = listBox1.Items.Count - 1;
                        }));
                    }
                    else
                    {
                        
                    }
                 
                }

                // Borrar el buffer y empezar a escuchar de nuevo
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
            // Codificar el mensaje y enviarlo al servidor.
            var msg = Encoding.ASCII.GetBytes(textBox1.Text);
            _client.GetStream().Write(msg, 0, msg.Length);

            // Borrar el cuadro de texto y ponerlo en foco
            textBox1.Text = "";
            textBox1.Focus();
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
