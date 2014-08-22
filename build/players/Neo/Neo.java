import java.util.Random;
public class Neo {
    public static void main(String[] args) {
        if(args.length < 2)
            return;
        String[] list = args[1].split(" ");
        if(list.length < 4)
            return;
        Random rand = new Random();
        int turn = list[0].split(",").length;
        if(turn == 49){
            System.out.print("S0");
            return;
        }
        int target=0;
        for(int i=1;i<4;i++)
            if(list[i].length()<2 || (list[i].charAt(0)!='-' && list[i].charAt(list[i].length()-2)!='S'))
                target=i;
        if(target>0){
            System.out.print("D"+target);
            return;
        }
        while(target<1){
            int i=rand.nextInt(3)+1;
            if(list[i].charAt(0)!='-')
                target=i;
        }
        System.out.print("S"+target);
    }
}
