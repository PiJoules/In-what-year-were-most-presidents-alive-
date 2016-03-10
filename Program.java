import java.util.Scanner;
import java.util.regex.Pattern;
import java.util.regex.Matcher;
import java.util.HashMap;
import java.util.Map;

public class Program {
	private static Pattern YEAR_PAT = Pattern.compile("\\d{4}");

	public static void main(String[] args){
		YearRange yr = new YearRange();
		Scanner scanner = new Scanner(System.in);
		while (scanner.hasNextLine()){
			String line = scanner.nextLine().trim();
			if (line.length() == 0){
				continue;
			}
			String[] parts = line.split(",");
			int born = parseDate(parts[1]);
			int death = parseDate(parts[3]);
			yr.merge(new YearRange(born, death));
		}
		scanner.close();

		// Find max. No default max function for Maps/
		int maxKey = 0, maxCount = 0;
		for (Map.Entry<Integer, Integer> entry : yr.getYears().entrySet()) {
			int year = entry.getKey();
			int count = entry.getValue();
			if (count > maxCount){
				maxCount = count;
				maxKey = year;
			}
		}

		System.out.println(maxKey + "," + maxCount);
	}

	private static int parseDate(String line){
		Matcher m = YEAR_PAT.matcher(line);
		if (m.find()){
			return Integer.parseInt(m.group(0));
		}
		return 9999;
	}

	private static class YearRange {
		private Map<Integer, Integer> years;

		public YearRange(){
			init(new HashMap<Integer, Integer>());
		}

		public YearRange(HashMap<Integer, Integer> years){
			init(years);
		}

		public YearRange(int start, int end){
			Map<Integer, Integer> years = new HashMap<Integer, Integer>();
			for (int i = start; i <= end; i++){
				years.put(i, 1);
			}
			init(years);
		}

		private void init(Map<Integer, Integer> years){
			this.years = years;
		}

		public Map<Integer, Integer> getYears(){
			return years;
		}

		public void merge(YearRange other){
			for (Map.Entry<Integer, Integer> entry : other.getYears().entrySet()) {
				int year = entry.getKey();
				int count = entry.getValue();
				Integer initYear = years.get(year);
				if (initYear == null){
					years.put(year, count);
				}
				else {
					years.put(year, years.get(year) + count);
				}
			}
		}
	}
}