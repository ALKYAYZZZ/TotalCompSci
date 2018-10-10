package aifolder;

public class Move{
    int value;
    int bin;

    public Move(int value1, int bin1){
        this.value = value1;
        this.bin = bin1;
    }

    public void setValue(int value1){
        this.value = value1;
    }

    public int getValue(){
        return this.value;
    }

    public void setBin(int bin1){
        this.bin = bin1;
    }

    public int getBin(){
        return this.bin;
    }
}