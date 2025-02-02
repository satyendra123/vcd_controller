import dotenv from 'dotenv';
import  bodyParser from 'body-parser';
import express from 'express';
dotenv.config();

const app = express();
const port = process.env.PORT || 3000;

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

app.post("/data", (req, res) => {
    console.log(req.body);
    res.send("Data received!");
  });

app.listen(port, () => {
    console.log(`Server listening at http://localhost:${port}`);
});

