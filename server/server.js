const express = require('express');
const app = express();
const port = 3000;

// Middleware to parse JSON bodies
app.use(express.json());

// Debugging middleware to log incoming requests
app.use((req, res, next) => {
  console.log(`[DEBUG] Request method: ${req.method}`);
  console.log(`[DEBUG] Request headers: ${JSON.stringify(req.headers)}`);
  console.log(`[DEBUG] Request body: ${JSON.stringify(req.body)}`);
  next();
});

// cities data
const citiesData = {
  damascus: { city: "Damascus", temperature: 20, description: "clear sky", humidity: 60, pressure: 1015 },
  aleppo: { city: "Aleppo", temperature: 25, description: "partly cloudy", humidity: 65, pressure: 1012 },
  paris: { city: "Paris", temperature: 18, description: "rainy", humidity: 70, pressure: 1013 },
  london: { city: "London", temperature: 15, description: "foggy", humidity: 80, pressure: 1009 },
  tokyo: { city: "Tokyo", temperature: 22, description: "sunny", humidity: 50, pressure: 1017 },
  idlib: { city: "idlib", temperature: 13, description: "sunny", humidity: 50, pressure: 1017 }
};

// Cities endpoint
app.post('/cities', (req, res) => {
  console.log(`[DEBUG] Parsed request body: ${JSON.stringify(req.body)}`);
  const { city, info } = req.body;

  if (!city) {
    return res.status(400).json({ error: "City is required" });
  }

  const cityData = citiesData[city.toLowerCase()];
  if (!cityData) {
    return res.status(404).json({ error: "City not found" });
  }

  if (info) {
    const requestedInfo = cityData[info.toLowerCase()];
    if (requestedInfo !== undefined) {
      return res.json({ [info]: requestedInfo });
    } else {
      return res.status(400).json({ error: "Requested info not available" });
    }
  }

  res.json(cityData);
});

// Start server
app.listen(port, () => {
  console.log(`Server is running on http://localhost:${port}`);
});