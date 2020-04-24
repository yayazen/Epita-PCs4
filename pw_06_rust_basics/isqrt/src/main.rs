fn main()
{
    for n in (0..=200).step_by(8) {
        
        println!("isqrt({0}) = {1}", n, isqrt(n));
    };
}

fn isqrt(n: u64) -> u64
{
    if n >= 2 {
        
        let r = 2 * isqrt(n >> 2)  + 1;

        if r * r > n { r - 1 } else { r }
    }
    else {

        n
    }
}

#[cfg(test)]
mod tests
{
    use super::*;

    #[test]
    fn test_isqrt()
    {
        assert_eq!(isqrt(0), 0);
        assert_eq!(isqrt(8), 2);
        assert_eq!(isqrt(24), 4);
        assert_eq!(isqrt(40), 6);
        assert_eq!(isqrt(48), 6);
        assert_eq!(isqrt(128), 11);
        assert_eq!(isqrt(168), 12);
        assert_eq!(isqrt(200), 14);
        assert_eq!(isqrt(u64::max_value()), 4_294_967_295);
    }
}

