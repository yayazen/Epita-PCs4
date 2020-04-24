use structopt::StructOpt;

#[derive(Debug, StructOpt)]
#[structopt(name = "Scrabble Score",
    about = "Find the score of a word for a Scrabble-like game.")]
struct Arg
{
    /// Mutiplies the score by two.
    #[structopt(short, long)]
    double: bool,

    /// Mutiplies the score by three.
    #[structopt(short, long)]
    triple: bool,

    /// Word to score.
    word: String,
}

fn main()
{
    let arg = Arg::from_args();
    dbg!(arg);
}
