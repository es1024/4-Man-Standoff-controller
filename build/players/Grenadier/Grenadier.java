public class Grenadier {
	public static void main(String[] args) {
		if(args.length < 2)
			return;
		String[] list = args[1].split(" ");
		if(list.length < 4)
			return;
		
		if(list[0].charAt(list[0].length()-1) != 'P'){
			System.out.print("P");
			return;
		}
		
		int target = 1;
		for(int i=2;i<4;i++)
			if(list[i].charAt(0)>list[target].charAt(0))
				target = i;
		
		System.out.print("T"+target);
	}
}

