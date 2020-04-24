use std::env;
use std::process;

#[derive(Debug)]
struct Arg
{
    double: bool,
    triple: bool,
    word: String,
}

fn main()
{
    let arg = get_arg();

    let mut score = 0;
    for c in arg.word.chars() {

        let val = match c {
          'a' | 'e' | 'i' | 'o' | 'u' | 'l' | 'n' | 'r' | 's' | 't' => 1,
          'd' | 'g' => 2,
          'b' | 'c' | 'm' | 'p' => 3,
          'f' | 'h' | 'v' | 'w' | 'y' => 4,
          'k' => 5,
          'j' | 'x' => 8,
          'q' | 'z' => 10,
          _ => 0
        };
        
        score += val;

        println!("{0}: {1}", c, val);
    }

    if arg.double { 
        score *= 2;
        println!("x2");
    }
    if arg.triple {
        score *= 3;
        println!("x3");
    }

    println!("Score = {}", score);
}

fn get_arg() -> Arg
{
    let mut arg = Arg
    {
        double: false,
        triple: false,
        word: String::new(),
    };


    for s in env::args().skip(1) {
        
        match &s as &str {
            "-dt" | "-td" => { arg.double = true; arg.triple = true; },
            "-d" | "--double" => arg.double = true,
            "-t" | "--triple" => arg.triple = true,
            _ => arg.word = s
        }
    }


    if !arg.word.is_empty() { arg }
    else {
        eprintln!("The word to score is missing.");
        process::exit(1);
    }
}
