using UnityEngine;
using System.Net.Sockets;
using System.Text;
using System.Net;
using System.Collections.Concurrent;
using System.Threading;

public class SocketClient : MonoBehaviour
{
    private byte[] buffer = new byte[1024];
    private Socket serverSocket;
    private Socket clientSocket;
    private const int MAX_MESSAGES_PER_FRAME = 50;
    private ConcurrentQueue<string> messageQueue = new ConcurrentQueue<string>();

    // 引用需要发送信息的对象
    public GameObject objectToSend;

    private void Start()
    {
        serverSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        serverSocket.Bind(new IPEndPoint(IPAddress.Any, 8888));
        serverSocket.Listen(1);
        Debug.Log("等待连接...");
        serverSocket.BeginAccept(new System.AsyncCallback(AcceptCallback), null);
    }

    private void AcceptCallback(System.IAsyncResult ar)
    {
        clientSocket = serverSocket.EndAccept(ar);
        Debug.Log("连接成功！");

        clientSocket.BeginReceive(buffer, 0, buffer.Length, SocketFlags.None, new System.AsyncCallback(ReceiveCallback), this);
    }

    private void ReceiveCallback(System.IAsyncResult ar)
    {
        int length = clientSocket.EndReceive(ar);
        byte[] message = new byte[length];
        System.Buffer.BlockCopy(buffer, 0, message, 0, length);

        string msg = Encoding.ASCII.GetString(message);
        messageQueue.Enqueue(msg); // 将消息添加到队列中

        clientSocket.BeginReceive(buffer, 0, buffer.Length, SocketFlags.None, new System.AsyncCallback(ReceiveCallback), this);
    }

    void Update()
    {
        int messagesProcessed = 0;
        while (messagesProcessed < MAX_MESSAGES_PER_FRAME && messageQueue.TryDequeue(out string msg))
        {
            Debug.Log("收到消息：" + msg);
            if (msg == "W") {
                objectToSend.GetComponent<Rigidbody>().AddForce(Vector3.forward * 200);
            }
            else if (msg == "S") {
                objectToSend.GetComponent<Rigidbody>().AddForce(Vector3.back * 200);
            }
            else if (msg == "A") {
                objectToSend.GetComponent<Rigidbody>().AddForce(Vector3.left * 200);
            }
            else if (msg == "D") {
                objectToSend.GetComponent<Rigidbody>().AddForce(Vector3.right * 200);
            }
            else if (msg == "Shift") {
                objectToSend.GetComponent<Rigidbody>().AddForce(Vector3.up * 1000);
            }
            messagesProcessed++;
        }

        // 发送需要发送对象的信息
        if (clientSocket != null && clientSocket. Connected && objectToSend != null) {
            string message = "Position: " + objectToSend.transform.position.ToString() + "\n"
                             + "Rotation: " + objectToSend.transform.rotation.ToString() + "\n"
                             + "Gravity: " + Physics.gravity.ToString() + "\n"; 
            SendMessage(message);
        }
        
    }
    
    private void SendMessage(string message)
    {
        byte[] data = Encoding.ASCII.GetBytes(message);
        clientSocket.BeginSend(data, 0, data.Length, SocketFlags.None, new System.AsyncCallback(SendCallback), clientSocket);
        //Debug.Log("发送消息：" + message);
    }
    
    private void SendCallback(System.IAsyncResult ar)
    {
        
        clientSocket.EndSend(ar);
    }
}
