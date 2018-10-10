public class NodeIterator<T extends Shape> implements java.util.Iterator<T>{
    Node<T> node;
    public NodeIterator(Node<T> n){
        node = n;
    }
        //Override iterator to accept node values rather than normal iterations.
        @Override
        public boolean hasNext(){
            if(this.node.next == null){
                return false;
            }
            else{
                return true;
            }
        }
        
        @Override
        public T next(){
            this.node = this.node.next;
            return this.node.v;
        }

        @Override
        public void remove() {
            throw new UnsupportedOperationException();
        }



}