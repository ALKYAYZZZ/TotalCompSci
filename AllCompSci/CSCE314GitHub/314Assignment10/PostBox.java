import java.util.*;
import java.util.concurrent.locks.*;
 
class PostBox implements Runnable {
    static final int MAX_SIZE = 10;
 
    class Message {
        String sender;
        String recipient;
        String msg;
        Message(String sender, String recipient, String msg) {
            this.sender = sender;
            this.recipient = recipient;
            this.msg = msg;
        }
    }
 
    private final LinkedList<Message> messages;
    private LinkedList<Message> myMessages;
    private String myId;
    private boolean stop = false;
	private Lock boxLock;
 
    public PostBox(String myId) {
        messages = new LinkedList<Message>();
        this.myId = myId;
        this.myMessages = new LinkedList<Message>();
        new Thread(this).start();
		this.boxLock = new ReentrantLock();
    }
 
    public PostBox(String myId, PostBox p) {
        this.myId = myId;
        this.messages = p.messages;
        this.myMessages = new LinkedList<Message>();
        new Thread(this).start();
		this.boxLock = p.boxLock;
    }
 
    public String getId() { return myId; }
 
    public void stop() {
        this.stop = true;
    }
 
    public void send(String recipient, String msg) {
        // add a message to the shared message queue
		boxLock.lock();
		
		try{
			messages.add(new Message(myId, recipient, msg));
		}
		finally{
			boxLock.unlock();
		}
    }
 
    public List<String> retrieve() {
        // return the contents of myMessages
        // and empty myMessages
		List<String> items = new LinkedList<String>();
		//Lock and convert messages to strings
		boxLock.lock();
		
		try{
			for(int i = 0; i < myMessages.size(); i++){
				Message j = myMessages.get(i);
				String jst = "From: " + j.sender + "To: " + j.recipient + ": " + j.msg + "\n";
				items.add(jst);
				
			}
			myMessages.clear();
				
		}
		finally{
			boxLock.unlock();
		}
		
		return items;
    }
 
    public void run() {
        // loop forever
        //   1. approximately once every second move all messages
        //      addressed to this post box from the shared message
        //      queue to the private myMessages queue
        //   2. also approximately once every second, if the message
        //      queue has more than MAX_SIZE messages, delete oldest messages
        //      so that size is at most MAX_SIZE
		
		while(stop == false){
			try{
				Thread.sleep(1000);
			}
			catch(
				InterruptedException e
			)
			{}
			//Lock and add and remove messages from queue
			boxLock.lock();
			
			List<Message> jst = new LinkedList<Message>();
			try{
				for(Message jst1: messages){
					if(jst1.recipient.equals(myId)){
						jst.add(jst1);
					}
				}
				for(Message jst2: jst){
					myMessages.add(jst2);
					messages.remove(jst2);
				}
				
				if(messages.size() > MAX_SIZE){
					while(messages.size() > MAX_SIZE){
						messages.remove();
					}
				}
			}
			finally{
				boxLock.unlock();
			}
		}	
	}
}
