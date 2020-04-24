pub fn divisor_sum(n: u64) -> u64
{
    let mut sum = (n != 1) as u64;
    let mut i = 2;

    loop {
        
        let d = n/i;

        if i > d {
            
            break sum
        }
        else if i * d == n {
            
            sum += if d != i { i + d } else { i };
        }

        i += 1;
    }
}

#[cfg(test)]
mod tests
{
    use super::*;

    #[test]
    fn test_divisor_sum()
    {
        assert_eq!(divisor_sum(1), 0);
        assert_eq!(divisor_sum(3), 1);
        assert_eq!(divisor_sum(6), 6);
        assert_eq!(divisor_sum(11), 1);
        assert_eq!(divisor_sum(30), 42);
        assert_eq!(divisor_sum(496), 496);
        assert_eq!(divisor_sum(8_589_869_056), 8_589_869_056);
        assert_eq!(divisor_sum(137_438_691_328), 137_438_691_328);
    }
}
