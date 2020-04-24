fn main()
{
    let s = reverse("Hello, world!");
    dbg!(s);
}

fn reverse(s: &str) -> String
{
    s.chars().rev().collect()
}

#[cfg(test)]
mod tests
{
    use super::*;

    #[test]
    fn test_reverse()
    {
        assert_eq!(reverse("Hello, world!"), "!dlrow ,olleH");
        assert_eq!(reverse("racecar"), "racecar");
        assert_eq!(reverse("I did-did I"), "I did-did I");

    }
}
