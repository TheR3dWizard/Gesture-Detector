const express = require("express");
const axios = require("axios");
require("dotenv").config();

const app = express();
const PORT = process.env.PORT || 5000;
const THINGSPEAK_API_URL = `https://api.thingspeak.com/channels/2862615/feeds.json?api_key=LE5SZKSCE1S0MH7H&results=10`;

app.use(require("cors")());
app.use(express.json());

const THRESHOLD = 250000; 


const predefinedGestures = [
    { name: "A", values: [180, 180, 180, 180, 180] },
    { name: "B", values: [180, 0, 0, 0, 0] },
    { name: "C", values: [100, 100, 100, 100, 100] },
    { name: "D", values: [180, 180, 0, 0, 0] },
    { name: "E", values: [0, 0, 0, 0, 0] },
    { name: "F", values: [90, 90, 180, 180, 180] },
    { name: "G", values: [180, 180, 90, 90, 90] },
    { name: "H", values: [180, 180, 90, 90, 180] },
    { name: "I", values: [180, 0, 0, 0, 180] },
    { name: "J", values: [180, 0, 0, 0, 180] },  
    { name: "K", values: [180, 90, 90, 0, 0] },
    { name: "L", values: [180, 0, 180, 180, 180] },
    { name: "M", values: [0, 0, 0, 180, 180] },
    { name: "N", values: [0, 0, 0, 90, 180] },
    { name: "O", values: [120, 120, 120, 120, 120] },
    { name: "P", values: [180, 180, 0, 90, 90] },
    { name: "Q", values: [180, 180, 90, 90, 180] },
    { name: "R", values: [180, 180, 90, 90, 0] },
    { name: "S", values: [0, 0, 0, 0, 0] },  
    { name: "T", values: [90, 90, 90, 0, 0] },
    { name: "U", values: [180, 180, 0, 0, 180] },
    { name: "V", values: [180, 180, 0, 0, 0] },
    { name: "W", values: [180, 180, 180, 0, 0] },
    { name: "X", values: [180, 90, 0, 0, 0] },
    { name: "Y", values: [180, 0, 180, 180, 180] },
    { name: "Z", values: [180, 180, 90, 90, 0] } 
];



function calculateCost(inputValues, referenceValues) {
    let cost = 0;
    for (let i = 0; i < 5; i++) {
        cost += Math.pow(inputValues[i] - referenceValues[i], 2);
    }
    return Math.abs(cost);
}


function recognizeGesture(inputValues) {
    let detectedGesture = "Unknown Gesture";
    let minCost = Infinity;

    predefinedGestures.forEach(gesture => {
        let cost = calculateCost(inputValues, gesture.values);
        console.log(`Comparing with ${gesture.name}, Cost: ${cost}`);

        if (cost < THRESHOLD && cost < minCost) {
            minCost = cost;
            detectedGesture = gesture.name;
        }
    });

    return detectedGesture;
}

app.get("/getData", async (req, res) => {
    try {
        const response = await axios.get(THINGSPEAK_API_URL);
        const feeds = response.data.feeds;

        if (!feeds || feeds.length === 0) {
            return res.status(404).json({ error: "No data found" });
        }

        
        let latestValues = { field1: null, field2: null, field3: null, field4: null, field5: null };

        
        for (let i = feeds.length - 1; i >= 0; i--) {
            for (let j = 1; j <= 5; j++) {
                if (!latestValues[`field${j}`] && feeds[i][`field${j}`] !== null) {
                    latestValues[`field${j}`] = Number(feeds[i][`field${j}`]);
                }
            }
        }

        
        const inputValues = Object.values(latestValues);
        console.log("Latest Sensor Readings:", inputValues);

        
        const detectedGesture = recognizeGesture(inputValues);

        res.json({ rawData: latestValues, processedValues: inputValues, gesture: detectedGesture });
    } catch (error) {
        console.error("Error fetching data:", error);
        res.status(500).json({ error: "Error fetching data" });
    }
});

app.listen(PORT, () => console.log(`Server running on port ${PORT}`));



