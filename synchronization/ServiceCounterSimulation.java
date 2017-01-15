import java.util.concurrent.*;
import java.util.*;
import java.io.*;
public class ServiceCounterSimulation extends Thread {

    public static Semaphore customerReady = new Semaphore(0);
    public static Semaphore shopkeeperReady = new Semaphore(0);
    public static Semaphore accessQueue = new Semaphore(1);

    

   public static int numberOfFreeSlots ;
   public static int numOfCustomer;
public static int customerCount=0;
   public static long time;
public static int interArrivalTime;
public static int  threadServiceTime;

class Customer extends Thread {
  
  int id;
  boolean notPurchased=true;
  long startTime;
long serviceTime;
  long endTime;

  public void run() {  
    startTime=new Date().getTime()-time;
  
    while (notPurchased) { 
      try {
      accessQueue.acquire();  
      if (numberOfFreeSlots > 0) {  
        this.id=++customerCount;
        System.out.println("Customer " + this.id + " just enque in line "+(new Date().getTime()-time));
        numberOfFreeSlots--;  

        customerReady.release();  
        accessQueue.release();   
        try {
         
	       shopkeeperReady.acquire();  
         serviceTime=new Date().getTime()-time;
        notPurchased = false;
        this.purchaseThings();  
        } catch (InterruptedException ex) {}
      }   
      else  {  
        this.id=++customerCount;
        System.err.println("There are no free Slots. Customer " + this.id + " has left the shop. at"+(new Date().getTime()-time));
        accessQueue.release();  
        notPurchased=false; 
      }
     }
      catch (InterruptedException ex) {}
    endTime=new Date().getTime()-time;
    System.out.println("customer "+this.id+" start at "+startTime+" service at "+serviceTime+" endTime "+endTime);
    }
    
  }


  
  public void purchaseThings(){
    System.err.println("Customer " + this.id+ " is purchasing Things at"+(new Date().getTime()-time));
    try {
    sleep(threadServiceTime);
    } catch (InterruptedException ex) {}
  }

}

 



class Shopkeeper extends Thread {
  long sleepTime;
  long wakeTime;
  public Shopkeeper() {}
  
  public void run() {
    while(true) {  
      try {
        sleepTime=new Date().getTime()-time; 
      customerReady.acquire(); 
      wakeTime=new Date().getTime()-time;
      accessQueue.acquire(); 
        numberOfFreeSlots++; 
      shopkeeperReady.release();  
      accessQueue.release(); 
      this.serviceCustomer();  
       System.out.println("ShopKeeper   sleep at "+sleepTime+" wake at "+wakeTime);
    } catch (InterruptedException ex) {}
   
    }
  }

   
   
  public void serviceCustomer(){
     System.err.println("The shopkeeper is now servicing customer at"+(new Date().getTime()-time));
    try {
      sleep(threadServiceTime);
    } catch (InterruptedException ex){ }
  }
}       
  
 

  public static void main(String args[]) throws Exception{
    Scanner sc=new Scanner(System.in);
    System.out.print("Enter number of Customer ");
    numOfCustomer=sc.nextInt();
    System.out.print("Enter Queue Length for Customer  ");
    numberOfFreeSlots=sc.nextInt();
    System.out.print("Enter interArrivalTime of Customer ");
    interArrivalTime=sc.nextInt();
    System.out.print("Enter Service Time Required for Customer ");
    threadServiceTime=sc.nextInt();
    FileOutputStream f = new FileOutputStream("trace");
     
     System.setOut(new PrintStream(f));
    ServiceCounterSimulation shop = new ServiceCounterSimulation(); 
    time=new Date().getTime();
    shop.start();  
  }

  public void run(){   
 
 Shopkeeper shopkeeper=new Shopkeeper();

  shopkeeper.start();  


    
   for (int i=0; i<numOfCustomer; i++) {
     Customer customer = new Customer();
     Random rn = new Random();

  long j = rn.nextInt(interArrivalTime);
 try {
    sleep(j);
    } catch (InterruptedException ex) {}
     customer.start();
    
   }
  } 
}