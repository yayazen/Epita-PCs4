use structopt::StructOpt;

const SINGLE: [&str; 10] = ["zero", "one", "two", "three", "four", "five", "six",
                        "seven", "eight", "nine"];
                  
const DOUBLE: [&str; 10] = ["ten", "eleven", "twelve", "thirteen","fourteen", 
                        "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"];

const MULT_10: [&str; 10] = ["", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", 
                        "eighty", "ninety"];

const POW_10: [&str; 6] = ["thousand", "million", "billion", "trillion", 
                        "quadrillion", "quintillion"];


#[derive(Debug, StructOpt)]
#[structopt(name = "Numbers to Words Converter",
    about = "Convert an integer into letters.")]
struct Arg
{
    /// A 64-bit unsigned integer
    number: u64
}

fn main()
{
    let arg = Arg::from_args();
    println!("{}", to_letters(arg.number));
}

fn to_letters(n: u64) -> String
{
    aux(n as usize, 0)
}

fn aux(n: usize, p: usize) -> String
{
    match n.to_string().len() {
        
        1 => SINGLE[n].to_string(),
        
        2 if n / 10 == 1 => DOUBLE[n % 10].to_string(),
        2 if n % 10 == 0 => MULT_10[n / 10].to_string(),
        2 => format!("{0}-{1}", MULT_10[n / 10], aux(n % 10, 0)),

        3 if n % 100 == 0 => format!("{} hundred", SINGLE[n / 100]),
        3 => format!("{0} hundred and {1}", SINGLE[n / 100], aux(n % 100, 0)),

        _ => {
            let mut s = String::from(&aux(n / 1000, p + 1));
            
            if (n / 1000) % 1000 != 0 {
                s.push_str(&format!(" {}", POW_10[p]));

            }

            if n % 1000 != 0 {
                s.push_str(&format!(" {}", aux(n % 1000, 0)))
            }

            s
        }
    }
}


#[cfg(test)]
mod tests
{
    use super::*;

    #[test]
    fn test_quintillions()
    {
        assert_eq!(to_letters(0), "zero");
        assert_eq!(to_letters(1), "one");
        assert_eq!(to_letters(2), "two");
        assert_eq!(to_letters(3), "three");
        assert_eq!(to_letters(4), "four");
        assert_eq!(to_letters(5), "five");
        assert_eq!(to_letters(6), "six");
        assert_eq!(to_letters(7), "seven");
        assert_eq!(to_letters(8), "eight");
        assert_eq!(to_letters(9), "nine");

        assert_eq!(to_letters(10), "ten");
        assert_eq!(to_letters(11), "eleven");
        assert_eq!(to_letters(12), "twelve");
        assert_eq!(to_letters(13), "thirteen");
        assert_eq!(to_letters(14), "fourteen");
        assert_eq!(to_letters(15), "fifteen");
        assert_eq!(to_letters(16), "sixteen");
        assert_eq!(to_letters(17), "seventeen");
        assert_eq!(to_letters(18), "eighteen");
        assert_eq!(to_letters(19), "nineteen");
        assert_eq!(to_letters(20), "twenty");
        assert_eq!(to_letters(21), "twenty-one");
        assert_eq!(to_letters(30), "thirty");
        assert_eq!(to_letters(32), "thirty-two");
        assert_eq!(to_letters(40), "forty");
        assert_eq!(to_letters(43), "forty-three");
        assert_eq!(to_letters(50), "fifty");
        assert_eq!(to_letters(54), "fifty-four");
        assert_eq!(to_letters(60), "sixty");
        assert_eq!(to_letters(65), "sixty-five");
        assert_eq!(to_letters(70), "seventy");
        assert_eq!(to_letters(76), "seventy-six");
        assert_eq!(to_letters(80), "eighty");
        assert_eq!(to_letters(87), "eighty-seven");
        assert_eq!(to_letters(90), "ninety");
        assert_eq!(to_letters(98), "ninety-eight");

        assert_eq!(to_letters(100), "one hundred");
        assert_eq!(to_letters(101), "one hundred and one");
        assert_eq!(to_letters(115), "one hundred and fifteen");
        assert_eq!(to_letters(165), "one hundred and sixty-five");
        assert_eq!(to_letters(200), "two hundred");
        assert_eq!(to_letters(277), "two hundred and seventy-seven");
        assert_eq!(to_letters(580), "five hundred and eighty");
        assert_eq!(to_letters(999), "nine hundred and ninety-nine");

        assert_eq!(to_letters(1_000), "one thousand");
        assert_eq!(to_letters(5_454), "five thousand four hundred and fifty-four");
        assert_eq!(to_letters(9_999), "nine thousand nine hundred and ninety-nine");

        assert_eq!(to_letters(100_002), "one hundred thousand two");
        assert_eq!(to_letters(200_100_003), "two hundred million one hundred thousand three");

        assert_eq!(to_letters(18_446_744_073_709_551_615),
            "eighteen quintillion four hundred and forty-six \
             quadrillion seven hundred and forty-four trillion \
             seventy-three billion \
             seven hundred and nine million \
             five hundred and fifty-one thousand \
             six hundred and fifteen");
    }
}
