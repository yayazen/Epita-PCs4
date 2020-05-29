fn main()
{
    println!("get_min(&[5, 4, 3, 2, 1]) = {}",
        get_min(&[5, 4, 3, 2, 1]));

    println!("remove_space(\"  No s pa  ce any more  \") = \"{}\"",
        remove_space("  No s pa  ce any more  "));

    println!("remove_odd(&[1, 2, 3, 4, 5]) = {:?}",
        remove_odd(&[1, 2, 3, 4, 5]));

    println!("remove_vowel(\"I don't understand!\") = \"{}\"",
        remove_vowel("I don't understand!"));

    println!("initialism(\"test driven development\") = \"{}\"",
        initialism("test driven development"));

    println!("count_vowel(\"This string contains 7 vowels.\") = {}",
        count_vowel("This string contains 7 vowels."));

    println!("space_to_underscore(\"avoid underscores in filenames\") = {}",
        space_to_underscore("avoid underscores in filenames"));

    let mut a = [1, 2, 3, 4, 5, 6, 7, 8, 9];
    println!("a = {:?}", a);
    println!("swap_chunks_mut(&mut a)");
    swap_chunks_mut(&mut a);
    println!("a = {:?}", a);
}

fn get_min(a: &[i32]) -> i32
{
    *a.iter().min().unwrap()
}

fn remove_space(s: &str) -> String
{
    s
    .chars()
    .filter(|&c| !c.is_whitespace())
    .collect()
}

fn remove_odd(a: &[i32]) -> Vec<i32>
{
    a
    .iter()
    .filter(|&&x| x % 2 == 0)
    .map(|&x| x)
    .collect::<Vec<_>>()
}

fn is_vowel(c: char) -> bool
{
    match c.to_ascii_lowercase() {

        'a' | 'e' | 'i' | 'o' | 'u' => true,
        _ => false
    }
}

fn count_vowel(s: &str) -> usize
{
    s
    .chars()
    .filter(|&c| is_vowel(c))
    .count()
}

fn remove_vowel(s: &str) -> String
{
    s
    .chars()
    .filter(|&c| !is_vowel(c))
    .collect()
}

fn initialism(s: &str) -> String
{
    s
    .split_whitespace()
    .map(|w| w
         .chars()
         .next()
         .unwrap()
         .to_ascii_uppercase()
    )
    .collect()
}

fn space_to_underscore(s: &str) -> String
{
    s
    .chars()
    .map(|c| if c.is_whitespace() { '_' } else { c }) 
    .collect()
        
}

fn swap_chunks_mut(a: &mut[u8])
{
    a
    .chunks_exact_mut(2)
    .for_each(|k| k.swap(0, 1))
}

#[cfg(test)]
mod tests
{
    use super::*;

    #[test]
    fn test_get_min()
    {
        assert_eq!(get_min(&[1, 2, 10, -5, -18, 23]), -18);
        assert_eq!(get_min(&[1, 2]), 1);
        assert_eq!(get_min(&[-5, 5]), -5);
    }

    #[test]
    fn test_remove_odd()
    {
        assert_eq!(remove_odd(&[1, 2, 10, -5, -18, 23]), [2, 10, -18]);
        assert_eq!(remove_odd(&[1, 2]), [2]);
        assert_eq!(remove_odd(&[-5, 5]), []);
    }

    #[test]
    fn test_remove_space()
    {
        assert_eq!(remove_space("Hello World"), "HelloWorld");
        assert_eq!(remove_space("I'am not a number"), "I'amnotanumber");
        assert_eq!(remove_space("    Good    bye!    "), "Goodbye!");
    }

    #[test]
    fn test_is_vowel()
    {
        assert!(is_vowel('a'));
        assert!(is_vowel('e'));
        assert!(is_vowel('i'));
        assert!(is_vowel('o'));
        assert!(is_vowel('u'));

        assert!(is_vowel('A'));
        assert!(is_vowel('E'));
        assert!(is_vowel('I'));
        assert!(is_vowel('O'));
        assert!(is_vowel('U'));

        assert!(!is_vowel('b'));
        assert!(!is_vowel('C'));
        assert!(!is_vowel('m'));
        assert!(!is_vowel('M'));
        assert!(!is_vowel('.'));
        assert!(!is_vowel('0'));
        assert!(!is_vowel('9'));
        assert!(!is_vowel(' '));
    }

    #[test]
    fn test_count_vowel()
    {
        assert_eq!(count_vowel("Hello World"), 3);
        assert_eq!(count_vowel("I'am not a number"), 6);
        assert_eq!(count_vowel("Good bye!"), 3);
    }

    #[test]
    fn test_remove_vowel()
    {
        assert_eq!(remove_vowel("Hello World"), "Hll Wrld");
        assert_eq!(remove_vowel("I'am not a number"), "'m nt  nmbr");
        assert_eq!(remove_vowel("Good bye!"), "Gd by!");
    }

    #[test]
    fn test_initialism()
    {
        assert_eq!(initialism("acquired immune deficiency syndrome"), "AIDS");
        assert_eq!(initialism("british broadcasting company"), "BBC");
        assert_eq!(initialism("cascading Style Sheets"), "CSS");
        assert_eq!(initialism("gnu compiler collection"), "GCC");
        assert_eq!(initialism("european space agency"), "ESA");
    }

    #[test]
    fn test_space_to_underscore()
    {
        assert_eq!(space_to_underscore("  abcd  "), "__abcd__");
        assert_eq!(space_to_underscore("my filename"), "my_filename");
        assert_eq!(space_to_underscore(" a    b c  "), "_a____b_c__");
        assert_eq!(space_to_underscore("        "), "________");
        assert_eq!(space_to_underscore("a      b"), "a______b");
    }

    #[test]
    fn test_swap_chunks_mut()
    {
        let mut a = [1, 2, 3, 4, 5, 6];

        swap_chunks_mut(&mut a);
        assert_eq!(a, [2, 1, 4, 3, 6, 5]);

        swap_chunks_mut(&mut a);
        assert_eq!(a, [1, 2, 3, 4, 5, 6]);

        let mut a = [1, 2, 3, 4, 5, 6, 7];

        swap_chunks_mut(&mut a);
        assert_eq!(a, [2, 1, 4, 3, 6, 5, 7]);

        swap_chunks_mut(&mut a);
        assert_eq!(a, [1, 2, 3, 4, 5, 6, 7]);
    }
}
