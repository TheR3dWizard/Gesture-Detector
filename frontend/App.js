import React, { useState, useEffect } from "react";

const App = () => {
  const [gesture, setGesture] = useState("");
  const [rawData, setRawData] = useState(null);
  const [processedValues, setProcessedValues] = useState([]);
  const [loading, setLoading] = useState(false);

  useEffect(() => {
    fetchGestureData();
  }, []);

  const fetchGestureData = async () => {
    setLoading(true);
    try {
      const response = await fetch("http://localhost:5000/getData");
      const data = await response.json();

      setGesture(data.gesture);
      setRawData(data.rawData);
      setProcessedValues(data.processedValues);
    } catch (error) {
      console.error("Error fetching gesture data:", error);
    }
    setLoading(false);
  };

  const Hand = () => (
    <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
      <path d="M18 11V6a2 2 0 0 0-2-2v0a2 2 0 0 0-2 2v0"></path>
      <path d="M14 10V4a2 2 0 0 0-2-2v0a2 2 0 0 0-2 2v2"></path>
      <path d="M10 10.5V6a2 2 0 0 0-2-2v0a2 2 0 0 0-2 2v8"></path>
      <path d="M18 8a2 2 0 1 1 4 0v6a8 8 0 0 1-8 8h-2c-2.8 0-4.5-.86-5.99-2.34l-3.6-3.6a2 2 0 0 1 2.83-2.82L7 15"></path>
    </svg>
  );

  const RefreshCw = ({ className }) => (
    <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round" className={className}>
      <path d="M3 12a9 9 0 0 1 9-9 9.75 9.75 0 0 1 6.74 2.74L21 8"></path>
      <path d="M21 3v5h-5"></path>
      <path d="M21 12a9 9 0 0 1-9 9 9.75 9.75 0 0 1-6.74-2.74L3 16"></path>
      <path d="M3 21v-5h5"></path>
    </svg>
  );

  const CheckCircle = () => (
    <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
      <path d="M22 11.08V12a10 10 0 1 1-5.93-9.14"></path>
      <polyline points="22 4 12 14.01 9 11.01"></polyline>
    </svg>
  );

  const Database = () => (
    <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
      <ellipse cx="12" cy="5" rx="9" ry="3"></ellipse>
      <path d="M21 12c0 1.66-4 3-9 3s-9-1.34-9-3"></path>
      <path d="M3 5v14c0 1.66 4 3 9 3s9-1.34 9-3V5"></path>
    </svg>
  );

  const BarChart3 = () => (
    <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
      <path d="M3 3v18h18"></path>
      <path d="M18 17V9"></path>
      <path d="M13 17V5"></path>
      <path d="M8 17v-3"></path>
    </svg>
  );

  return (
    <div className="app-container">
      <header className="app-header">
        <h1 className="header-title">
          <span className="icon-wrapper"><Hand /></span>
          Gesture Detection
        </h1>
      </header>

      <main className="content">
        <div className="card">
          <div className="card-header">
            <h2 className="card-title">Gesture Analysis Dashboard</h2>
          </div>
          
          <div className="card-content">
            {loading ? (
              <div className="loading-container">
                <div className="loading-indicator">
                  <RefreshCw className="spin-icon" />
                  <span className="loading-text">Loading data...</span>
                </div>
                <div className="skeleton"></div>
                <div className="skeleton tall"></div>
                <div className="skeleton"></div>
              </div>
            ) : (
              <div className="data-container">
                <div className="data-section">
                  <div className="section-header">
                    <span className="icon-wrapper small"><CheckCircle /></span>
                    <h3 className="section-title">Detected Gesture:</h3>
                  </div>
                  <div className="data-display">
                    <p className="gesture-text">
                      {gesture || "None detected"}
                      <span className="badge">Detected</span>
                    </p>
                  </div>
                </div>

                <div className="data-section">
                  <div className="section-header">
                    <span className="icon-wrapper small"><Database /></span>
                    <h3 className="section-title">Raw Sensor Data:</h3>
                  </div>
                  <div className="raw-data">
                    {rawData ? JSON.stringify(rawData, null, 2) : "No Data"}
                  </div>
                </div>

                <div className="data-section">
                  <div className="section-header">
                    <span className="icon-wrapper small"><BarChart3 /></span>
                    <h3 className="section-title">Processed Values:</h3>
                  </div>
                  <div className="data-display">
                    <div className="badge-container">
                      {processedValues.length > 0 ? (
                        processedValues.map((value, index) => (
                          <span key={index} className="badge value-badge">
                            {value}
                          </span>
                        ))
                      ) : (
                        <p className="no-data">No processed values available</p>
                      )}
                    </div>
                  </div>
                </div>
              </div>
            )}

            <div className="button-container">
              <button 
                onClick={fetchGestureData} 
                className={`refresh-button ${loading ? 'disabled' : ''}`}
                disabled={loading}
              >
                <span className="icon-wrapper small">
                  <RefreshCw className={loading ? 'spin' : ''} />
                </span>
                Refresh Data
              </button>
            </div>
          </div>
        </div>
      </main>
    </div>
  );
};

export default App;

// CSS Styles
const styles = `
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
  font-family: 'Inter', -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
}

.app-container {
  min-height: 100vh;
  background: linear-gradient(135deg, #ffc2d1, #ffd1c2);
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  padding: 1rem;
}

.app-header {
  text-align: center;
  margin-bottom: 1.5rem;
}

.header-title {
  font-size: 2.5rem;
  font-weight: 700;
  color: white;
  text-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
  display: flex;
  align-items: center;
  justify-content: center;
  gap: 0.5rem;
}

.content {
  max-width: 600px;
  width: 100%;
}

.card {
  background: white;
  border-radius: 0.75rem;
  overflow: hidden;
  box-shadow: 0 10px 25px rgba(0, 0, 0, 0.1);
  border: none;
}

.card-header {
  background: linear-gradient(to right, #ff6b8b, #ff8e6e);
  padding: 1rem;
  border-top-left-radius: 0.75rem;
  border-top-right-radius: 0.75rem;
}

.card-title {
  color: white;
  font-size: 1.25rem;
  font-weight: 600;
  text-align: center;
}

.card-content {
  padding: 1.5rem;
}

.data-container {
  display: flex;
  flex-direction: column;
  gap: 1.5rem;
}

.data-section {
  display: flex;
  flex-direction: column;
  gap: 0.5rem;
}

.section-header {
  display: flex;
  align-items: center;
  gap: 0.5rem;
}

.section-title {
  font-size: 1.125rem;
  font-weight: 600;
  color: #4b5563;
}

.data-display {
  background-color: #fff5f7;
  padding: 0.75rem;
  border-radius: 0.5rem;
}

.gesture-text {
  font-size: 1.5rem;
  font-weight: 700;
  color: #ff6b8b;
  display: flex;
  align-items: center;
  justify-content: space-between;
}

.raw-data {
  background-color: #1f2937;
  color: #f3f4f6;
  padding: 1rem;
  border-radius: 0.5rem;
  overflow: auto;
  max-height: 12rem;
  font-family: monospace;
  font-size: 0.875rem;
  white-space: pre-wrap;
}

.badge-container {
  display: flex;
  flex-wrap: wrap;
  gap: 0.5rem;
}

.badge {
  background-color: #fff5f7;
  color: #ff6b8b;
  border: 1px solid #fecdd3;
  padding: 0.25rem 0.5rem;
  border-radius: 9999px;
  font-size: 0.75rem;
  font-weight: 500;
}

.value-badge {
  background-color: #fff5f7;
  color: #ff6b8b;
  border: 1px solid #fecdd3;
}

.no-data {
  color: #9ca3af;
}

.button-container {
  margin-top: 1.5rem;
  display: flex;
  justify-content: center;
}

.refresh-button {
  background: linear-gradient(to right, #ff6b8b, #ff8e6e);
  color: white;
  border: none;
  padding: 0.75rem 1.5rem;
  border-radius: 9999px;
  font-size: 1rem;
  font-weight: 500;
  cursor: pointer;
  transition: all 0.3s ease;
  display: flex;
  align-items: center;
  gap: 0.5rem;
  box-shadow: 0 4px 6px rgba(255, 107, 139, 0.2);
}

.refresh-button:hover {
  transform: translateY(-2px);
  box-shadow: 0 6px 10px rgba(255, 107, 139, 0.3);
}

.refresh-button.disabled {
  opacity: 0.7;
  cursor: not-allowed;
}

.icon-wrapper {
  display: inline-flex;
  align-items: center;
  justify-content: center;
}

.icon-wrapper.small {
  width: 20px;
  height: 20px;
}

.icon-wrapper.small svg {
  width: 20px;
  height: 20px;
}

.loading-container {
  display: flex;
  flex-direction: column;
  gap: 1rem;
}

.loading-indicator {
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 2rem 0;
}

.loading-text {
  margin-left: 0.75rem;
  font-size: 1.125rem;
  font-weight: 500;
  color: #4b5563;
}

.skeleton {
  height: 2rem;
  background: linear-gradient(90deg, #f3f4f6 0%, #e5e7eb 50%, #f3f4f6 100%);
  background-size: 200% 100%;
  animation: shimmer 1.5s infinite;
  border-radius: 0.375rem;
}

.skeleton.tall {
  height: 8rem;
}

.spin, .spin-icon {
  animation: spin 1s linear infinite;
}

@keyframes spin {
  from { transform: rotate(0deg); }
  to { transform: rotate(360deg); }
}

@keyframes shimmer {
  0% { background-position: 200% 0; }
  100% { background-position: -200% 0; }
}
`;

// Add styles to document
(() => {
  const styleSheet = document.createElement("style");
  styleSheet.type = "text/css";
  styleSheet.innerText = styles;
  document.head.appendChild(styleSheet);
})();