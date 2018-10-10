import java.lang.Class;
import java.lang.reflect.*;
import static java.lang.System.err;
import java.util.*;
import java.io.*;

public class Main4{
	public static void main(String[] args){
		String cname = args[0];
		
		Class clss = null;
		try{
			clss = Class.forName(cname);
		}
		catch(ClassNotFoundException e) {
			
		}
		
		Method methodList[] = clss.getDeclaredMethods();
		
		for(Method n: methodList){
			if(isBoolTest(n) == true){
				Object o = null;
				try{
					o = n.invoke(null);
				}
				catch(Exception e){
					
				}
				if((boolean) o){
					System.out.println("OK: " + n.getName() + " succeeded");
					
				}
				else{
					System.out.println("Failed: " + n.getName() + " failed");
				}
				
			}
		}
	}
		
		static boolean isBoolTest(Method n){
			try{
				Type[] tList = n.getGenericParameterTypes();
				
				if(Modifier.isPublic(n.getModifiers())&&Modifier.isStatic(n.getModifiers())){
					if(n.getGenericParameterTypes().length == 0){
						if((n.getGenericReturnType().toString()).equals("boolean")){
							if(n.getName().startsWith("test")){
								return true;
							}
						} 
					}
				}
			}
			catch(Throwable e){
				System.err.println(e);
			}
			return false;
		}
}

class MyClass{
public static boolean test1(){return true;}
public static boolean test2(){return false;}
public static boolean testTAMU(boolean tamu){return tamu;}
public boolean testCSE(){return false;}
public static int testAggie(){return 2017;}
public static boolean fakeTestAggie(){return false;}
}