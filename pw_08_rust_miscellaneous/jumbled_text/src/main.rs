const SEPARATORS: &str = " ,;:!?./%*$=+)@_-('\"&1234567890\r\n";

fn main()
{
    let mut args = std::env::args().skip(1);

    let text = match args.next() {

        Some(s) if !args.next().is_some() => s,
        _ => String::from("This is the default text.")
    };

    println!("{}\n{}", text, mix(&text));
}

fn mix(s: &str) -> String
{
    let mut a: Vec<char> = s.chars().collect();
    
    a
    .split_mut(|c| SEPARATORS.contains(*c))
    .filter(|w| w.len() > 3)
    .for_each(|w| {
        let l = w.len() - 1;

        w[1..l]
        .chunks_exact_mut(2)
        .for_each(|k| k.swap(0, 1));
    });

    a.iter().collect()
}

#[cfg(test)]
mod tests
{
    use super::*;

    #[test]
    fn test_mix()
    {
        assert_eq!(mix("abcd"), "acbd");
        assert_eq!(mix("abcdefghij"), "acbedgfihj");
        assert_eq!(mix("This is the default text."), "Tihs is the dfeualt txet.");
        assert_eq!(mix("Hello World!"), "Hlelo Wrold!");
        assert_eq!(mix("I was born ready!"), "I was bron raedy!");
        assert_eq!(mix("I am not a number! I'm a free man!"), "I am not a nmuebr! I'm a fere man!");
        
        let text =  "According to a researcher at Cambridge University, \
                    it doesn't matter in what order the letters in a word are, \
                    the only important thing is that the first and last letters \
                    be at the right place. The rest can be a total mess and you \
                    can still read it without problem. This is because the human \
                    mind does not read every letter by itself but the word as a whole.";

        let jumbled_text = "Accroidng to a rseaecrehr at Cmarbdige Uinevsrtiy, \
                            it deosn't mtaetr in waht odrer the lteetrs in a wrod are, \
                            the olny ipmroatnt tihng is taht the frist and lsat lteetrs \
                            be at the rgiht palce. The rset can be a ttoal mses and you \
                            can sitll raed it wtiohut porlbem. Tihs is bceuase the hmuan \
                            mnid deos not raed eevry lteetr by istlef but the wrod as a wohle.";


        assert_eq!(mix(text), jumbled_text);

        
    }
}
