use goose::prelude::*;
use goose_eggs::{validate_page, Validate};

const NUM1: i32 = 2191983;
const NUM2: i32 = 43583759;

async fn loadtest_add(user: &mut GooseUser) -> TransactionResult {
    let path = format!("/add/{}/{}", NUM1, NUM2);
    let goose = user.get(&path).await?;

    let expected = format!("{} + {} = {}\n", NUM1, NUM2, NUM1 + NUM2);
    let validate = &Validate::builder().status(200).text(&expected).build();

    validate_page(user, goose, &validate).await?;

    Ok(())
}

#[tokio::main]
async fn main() -> Result<(), GooseError> {
    GooseAttack::initialize()?
        .register_scenario(
            scenario!("LoadtestTransactions").register_transaction(transaction!(loadtest_add)),
        )
        .set_default(GooseDefault::Host, "http://localhost:3030")?
        .execute()
        .await?;

    Ok(())
}
