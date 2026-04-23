const express = require("express")
const app = express()

let dados = [];

app.get("/sensor", (req,res) =>{
    const {temp, hum} = req.query;

    if(!temp || !hum){
        return res.status(400).send("Dados invalidos");
    }

    const leitura = {
        temperatura : temp,
        umidade : hum,
        data: new Date()
    };
    dados.push(leitura);

    console.log(`Temp: ${temp} | Umidade: ${hum}`);

    res.send("ok");
});