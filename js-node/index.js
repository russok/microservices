const express = require('express');
const app = express();
const port = 3030;

app.get('/add/:int1/:int2', async (req, res) => {
    const {int1, int2} = req.params;
    res.send(`${int1} + ${int2} = ${parseInt(int1) + parseInt(int2)}\n`);
});

app.listen(port, () => {
    console.log(`Node "add" service listening on port ${port}`);
});