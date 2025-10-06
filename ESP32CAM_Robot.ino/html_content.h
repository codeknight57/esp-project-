#ifndef HTML_CONTENT_H
#define HTML_CONTENT_H

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
    <title>Robot Control</title>
    <style>
        :root {
            --primary: #3498db;
            --secondary: #2ecc71;
            --danger: #e74c3c;
            --warning: #f39c12;
            --purple: #9b59b6;
            
            /* Light theme */
            --bg-primary: #ffffff;
            --bg-secondary: #f8f9fa;
            --bg-card: #ffffff;
            --text-primary: #2c3e50;
            --text-secondary: #7f8c8d;
            --border: #e1e4e8;
            --shadow: rgba(0, 0, 0, 0.1);
            --toggle-bg: #f1f1f1;
            --toggle-border: #ccc;
        }
        
        [data-theme="dark"] {
            /* Dark theme */
            --bg-primary: #1a1a2e;
            --bg-secondary: #16213e;
            --bg-card: #0f3460;
            --text-primary: #ecf0f1;
            --text-secondary: #bdc3c7;
            --border: #34495e;
            --shadow: rgba(0, 0, 0, 0.3);
            --toggle-bg: #2c3e50;
            --toggle-border: #34495e;
        }
        
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            -webkit-user-select: none; /* Prevent text selection */
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
            -webkit-touch-callout: none; /* Prevent iOS long press context menu */
            -webkit-tap-highlight-color: transparent; /* Remove tap highlight */
        }
        
        body {
            background-color: var(--bg-primary);
            color: var(--text-primary);
            min-height: 100vh;
            display: flex;
            flex-direction: column;
            align-items: center;
            padding: 10px;
            overflow-x: hidden;
            transition: background-color 0.3s ease, color 0.3s ease;
            touch-action: manipulation; /* Disable double-tap zoom */
        }
        
        .header {
            width: 100%;
            max-width: 500px;
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 10px;
            position: relative;
            gap: 8px;
        }
        
        .header-controls {
            display: flex;
            align-items: center;
            gap: 8px;
            flex: 1;
        }
        
        .settings-dropdown, .speed-dropdown {
            position: relative;
            display: inline-block;
        }
        
        .settings-btn, .speed-btn {
            background: var(--bg-card);
            border: 1px solid var(--border);
            border-radius: 8px;
            padding: 8px 12px;
            font-size: 0.9rem;
            cursor: pointer;
            display: flex;
            align-items: center;
            box-shadow: 0 2px 5px var(--shadow);
            color: var(--text-primary);
            transition: all 0.2s ease;
            white-space: nowrap;
            -webkit-user-select: none; /* Prevent text selection */
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
            touch-action: manipulation; /* Disable double-tap zoom */
        }
        
        .settings-btn:hover, .speed-btn:hover {
            background: var(--bg-secondary);
        }
        
        .settings-content, .speed-content {
            display: none;
            position: fixed;
            background-color: var(--bg-card);
            min-width: 250px;
            max-width: 90vw;
            box-shadow: 0 8px 16px var(--shadow);
            z-index: 1000;
            border-radius: 8px;
            border: 1px solid var(--border);
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            margin-top: 0;
            overflow: hidden;
            max-height: 80vh;
            overflow-y: auto;
        }
        
        .settings-content.show, .speed-content.show {
            display: block;
        }
        
        .setting-item, .speed-item {
            padding: 12px 15px;
            display: flex;
            justify-content: space-between;
            align-items: center;
            border-bottom: 1px solid var(--border);
            flex-wrap: wrap;
        }
        
        .setting-item:last-child, .speed-item:last-child {
            border-bottom: none;
        }
        
        .setting-label, .speed-label {
            font-size: 0.9rem;
            color: var(--text-primary);
            flex: 1;
            min-width: 100px;
            margin-right: 10px;
        }
        
        .setting-control, .speed-control {
            display: flex;
            align-items: center;
            flex: 1;
            justify-content: flex-end;
        }
        
        .setting-slider, .speed-slider {
            width: 100px;
            margin: 0 10px;
            flex: 1;
            max-width: 120px;
        }
        
        .setting-value, .speed-value {
            font-size: 0.8rem;
            color: var(--text-secondary);
            min-width: 30px;
            text-align: right;
            white-space: nowrap;
        }
        
        .setting-select {
            background: var(--bg-primary);
            color: var(--text-primary);
            border: 1px solid var(--border);
            border-radius: 4px;
            padding: 4px 8px;
            font-size: 0.8rem;
            max-width: 120px;
            white-space: nowrap;
            overflow: hidden;
            text-overflow: ellipsis;
        }
        
        .reset-button {
            background: linear-gradient(45deg, var(--danger), #c0392b);
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 15px;
            font-size: 0.9rem;
            font-weight: bold;
            cursor: pointer;
            transition: all 0.2s ease;
            box-shadow: 0 2px 5px var(--shadow);
            margin: 10px 15px;
            width: calc(100% - 30px);
            text-align: center;
            -webkit-user-select: none; /* Prevent text selection */
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
            touch-action: manipulation; /* Disable double-tap zoom */
        }
        
        .reset-button:hover {
            transform: translateY(-2px);
            box-shadow: 0 4px 8px var(--shadow);
        }
        
        .reset-button:active {
            transform: translateY(0);
            box-shadow: 0 1px 3px var(--shadow);
        }
        
        .theme-toggle {
            position: relative;
            width: 60px;
            height: 30px;
            background-color: var(--toggle-bg);
            border: 1px solid var(--toggle-border);
            border-radius: 30px;
            cursor: pointer;
            transition: all 0.3s ease;
            box-shadow: 0 2px 5px var(--shadow);
            display: flex;
            align-items: center;
            justify-content: space-between;
            padding: 0 5px;
            -webkit-user-select: none; /* Prevent text selection */
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
            touch-action: manipulation; /* Disable double-tap zoom */
        }
        
        .theme-icon {
            width: 20px;
            height: 20px;
            border-radius: 50%;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 12px;
            color: var(--text-secondary);
            transition: all 0.3s ease;
        }
        
        .theme-toggle .slider {
            position: absolute;
            top: 3px;
            left: 3px;
            width: 22px;
            height: 22px;
            border-radius: 50%;
            background-color: white;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);
            transition: all 0.3s ease;
            display: flex;
            align-items: center;
            justify-content: center;
        }
        
        [data-theme="dark"] .theme-toggle .slider {
            transform: translateX(30px);
            background-color: #1a1a2e;
        }
        
        .status-container {
            display: flex;
            justify-content: center;
            width: 100%;
            max-width: 500px;
            margin-bottom: 10px;
        }
        
        .status-box {
            background: var(--bg-card);
            border: 1px solid var(--border);
            border-radius: 10px;
            padding: 8px 15px;
            margin: 0 5px;
            flex: 1;
            text-align: center;
            box-shadow: 0 2px 5px var(--shadow);
            transition: transform 0.2s ease;
            min-width: 0;
        }
        
        .status-box:hover {
            transform: translateY(-2px);
        }
        
        .status-title {
            font-size: 0.8rem;
            color: var(--text-secondary);
            margin-bottom: 2px;
            white-space: nowrap;
            overflow: hidden;
            text-overflow: ellipsis;
        }
        
        .status-value {
            font-size: 1rem;
            font-weight: bold;
            color: var(--text-primary);
            white-space: nowrap;
            overflow: hidden;
            text-overflow: ellipsis;
        }
        
        .camera-container {
            position: relative;
            width: 100%;
            max-width: 500px;
            margin-bottom: 15px;
            border-radius: 10px;
            overflow: hidden;
            box-shadow: 0 5px 15px var(--shadow);
            border: 1px solid var(--border);
        }
        
        .camera-image {
            width: 100%;
            height: auto;
            display: block;
        }
        
        .controls {
            width: 100%;
            max-width: 500px;
        }
        
        .control-group {
            margin-bottom: 15px;
        }
        
        .control-title {
            text-align: center;
            margin-bottom: 10px;
            font-size: 1rem;
            color: var(--text-primary);
        }
        
        .direction-controls {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            grid-gap: 10px;
        }
        
        .btn {
            padding: 12px;
            border: none;
            border-radius: 8px;
            font-size: 1rem;
            font-weight: bold;
            cursor: pointer;
            transition: all 0.2s ease;
            box-shadow: 0 2px 5px var(--shadow);
            color: white;
            position: relative;
            overflow: hidden;
            z-index: 1;
            border: 1px solid transparent;
            white-space: nowrap;
            -webkit-user-select: none; /* Prevent text selection */
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
            -webkit-touch-callout: none; /* Prevent iOS long press context menu */
            touch-action: manipulation; /* Disable double-tap zoom */
            -webkit-tap-highlight-color: transparent; /* Remove tap highlight */
        }
        
        .btn:before {
            content: "";
            position: absolute;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background: rgba(255, 255, 255, 0.2);
            transform: translateX(-100%);
            transition: transform 0.3s ease;
            z-index: -1;
        }
        
        .btn:hover:before {
            transform: translateX(0);
        }
        
        .btn:active {
            transform: scale(0.95);
            box-shadow: 0 1px 3px var(--shadow);
        }
        
        .btn-forward {
            grid-column: 2;
            background: linear-gradient(45deg, var(--primary), #2980b9);
        }
        
        .btn-left {
            grid-column: 1;
            grid-row: 2;
            background: linear-gradient(45deg, var(--secondary), #27ae60);
        }
        
        .btn-light {
            grid-column: 2;
            grid-row: 2;
            background: linear-gradient(45deg, var(--warning), #e67e22);
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 0.9rem;
        }
        
        .btn-right {
            grid-column: 3;
            grid-row: 2;
            background: linear-gradient(45deg, var(--purple), #8e44ad);
        }
        
        .btn-backward {
            grid-column: 2;
            grid-row: 3;
            background: linear-gradient(45deg, var(--danger), #c0392b);
        }
        
        .servo-controls {
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 10px;
        }
        
        .btn-servo-minus, .btn-servo-plus {
            width: 50px;
            height: 50px;
            border-radius: 50%;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 1.2rem;
            -webkit-user-select: none; /* Prevent text selection */
            -moz-user-select: none;
            -ms-user-select: none;
            user-select: none;
            -webkit-touch-callout: none; /* Prevent iOS long press context menu */
            touch-action: manipulation; /* Disable double-tap zoom */
            -webkit-tap-highlight-color: transparent; /* Remove tap highlight */
        }
        
        .btn-servo-minus {
            background: linear-gradient(45deg, var(--danger), #c0392b);
        }
        
        .btn-servo-plus {
            background: linear-gradient(45deg, var(--primary), #2980b9);
        }
        
        .servo-display {
            background: var(--bg-card);
            border: 1px solid var(--border);
            border-radius: 8px;
            padding: 10px 15px;
            font-size: 1.2rem;
            font-weight: bold;
            min-width: 80px;
            text-align: center;
            box-shadow: 0 2px 5px var(--shadow);
            color: var(--text-primary);
        }
        
        .light-indicator {
            width: 12px;
            height: 12px;
            border-radius: 50%;
            background-color: var(--text-secondary);
            margin-left: 8px;
            transition: all 0.3s ease;
        }
        
        .light-on {
            background-color: #f1c40f;
            box-shadow: 0 0 8px #f1c40f;
        }
        
        @media (max-width: 500px) {
            .header {
                justify-content: space-between;
                flex-wrap: nowrap;
            }
            
            .header-controls {
                gap: 5px;
            }
            
            .settings-btn, .speed-btn {
                padding: 6px 10px;
                font-size: 0.8rem;
            }
            
            .theme-toggle {
                width: 50px;
                height: 25px;
            }
            
            .theme-toggle .slider {
                width: 19px;
                height: 19px;
                top: 3px;
                left: 3px;
            }
            
            [data-theme="dark"] .theme-toggle .slider {
                transform: translateX(25px);
            }
            
            .status-box {
                padding: 6px 10px;
            }
            
            .status-title {
                font-size: 0.7rem;
            }
            
            .status-value {
                font-size: 0.9rem;
            }
            
            .btn {
                padding: 10px;
                font-size: 0.9rem;
            }
            
            .btn-light {
                font-size: 0.8rem;
            }
            
            .btn-servo-minus, .btn-servo-plus {
                width: 45px;
                height: 45px;
                font-size: 1.1rem;
            }
            
            .servo-display {
                padding: 8px 12px;
                font-size: 1.1rem;
                min-width: 70px;
            }
            
            .settings-content, .speed-content {
                min-width: 180px;
                max-width: 95vw;
            }
            
            .setting-slider, .speed-slider {
                width: 80px;
                max-width: 100px;
            }
            
            .setting-select {
                max-width: 100px;
                font-size: 0.75rem;
            }
        }
        
        @media (max-width: 350px) {
            .header {
                gap: 5px;
            }
            
            .settings-btn, .speed-btn {
                padding: 5px 8px;
                font-size: 0.7rem;
            }
            
            .theme-toggle {
                width: 45px;
                height: 22px;
            }
            
            .theme-toggle .slider {
                width: 18px;
                height: 18px;
                top: 2px;
                left: 2px;
            }
            
            [data-theme="dark"] .theme-toggle .slider {
                transform: translateX(23px);
            }
            
            .direction-controls {
                grid-gap: 8px;
            }
            
            .btn {
                padding: 8px;
                font-size: 0.8rem;
            }
            
            .btn-light {
                font-size: 0.7rem;
            }
            
            .btn-servo-minus, .btn-servo-plus {
                width: 40px;
                height: 40px;
                font-size: 1rem;
            }
            
            .servo-display {
                padding: 6px 10px;
                font-size: 1rem;
                min-width: 60px;
            }
            
            .settings-content, .speed-content {
                min-width: 160px;
                max-width: 98vw;
            }
            
            .setting-slider, .speed-slider {
                width: 60px;
                max-width: 80px;
            }
            
            .setting-select {
                max-width: 80px;
                font-size: 0.7rem;
            }
            
            .setting-item, .speed-item {
                padding: 10px 12px;
            }
            
            .setting-label, .speed-label {
                font-size: 0.8rem;
                min-width: 80px;
            }
            
            .reset-button {
                font-size: 0.8rem;
                padding: 6px 12px;
            }
        }
    </style>
</head>
<body>
    <div class="header">
        <div class="header-controls">
            <div class="settings-dropdown">
                <button class="settings-btn" onclick="toggleSettings()">
                    Camera &#9660;
                </button>
                <div id="settingsDropdown" class="settings-content">
                    <div class="setting-item">
                        <span class="setting-label">Brightness</span>
                        <div class="setting-control">
                            <input type="range" min="-2" max="2" value="0" class="setting-slider" id="brightness" oninput="updateCameraSetting('brightness', this.value)">
                            <span class="setting-value" id="brightnessValue">0</span>
                        </div>
                    </div>
                    <div class="setting-item">
                        <span class="setting-label">Contrast</span>
                        <div class="setting-control">
                            <input type="range" min="-2" max="2" value="0" class="setting-slider" id="contrast" oninput="updateCameraSetting('contrast', this.value)">
                            <span class="setting-value" id="contrastValue">0</span>
                        </div>
                    </div>
                    <div class="setting-item">
                        <span class="setting-label">Saturation</span>
                        <div class="setting-control">
                            <input type="range" min="-2" max="2" value="0" class="setting-slider" id="saturation" oninput="updateCameraSetting('saturation', this.value)">
                            <span class="setting-value" id="saturationValue">0</span>
                        </div>
                    </div>
                    <div class="setting-item">
                        <span class="setting-label">Sharpness</span>
                        <div class="setting-control">
                            <input type="range" min="-2" max="2" value="0" class="setting-slider" id="sharpness" oninput="updateCameraSetting('sharpness', this.value)">
                            <span class="setting-value" id="sharpnessValue">0</span>
                        </div>
                    </div>
                    <div class="setting-item">
                        <span class="setting-label">Special Effect</span>
                        <div class="setting-control">
                            <select id="specialEffect" onchange="updateCameraSetting('special_effect', this.value)" class="setting-select">
                                <option value="0">None</option>
                                <option value="1">Negative</option>
                                <option value="2">Grayscale</option>
                                <option value="3">Red Tint</option>
                                <option value="4">Green Tint</option>
                                <option value="5">Blue Tint</option>
                                <option value="6">Sepia</option>
                            </select>
                        </div>
                    </div>
                    <div class="setting-item">
                        <span class="setting-label">White Balance</span>
                        <div class="setting-control">
                            <select id="whiteBalance" onchange="updateCameraSetting('wb_mode', this.value)" class="setting-select">
                                <option value="0">Auto</option>
                                <option value="1">Sunny</option>
                                <option value="2">Cloudy</option>
                                <option value="3">Office</option>
                                <option value="4">Home</option>
                            </select>
                        </div>
                    </div>
                    <div class="setting-item">
                        <span class="setting-label">Exposure</span>
                        <div class="setting-control">
                            <input type="range" min="-2" max="2" value="0" class="setting-slider" id="exposure" oninput="updateCameraSetting('exposure', this.value)">
                            <span class="setting-value" id="exposureValue">0</span>
                        </div>
                    </div>
                    <button class="reset-button" onclick="resetCameraSettings()">Reset to Default</button>
                </div>
            </div>
            
            <div class="speed-dropdown">
                <button class="speed-btn" onclick="toggleSpeedSettings()">
                    Speed &#9660;
                </button>
                <div id="speedDropdown" class="speed-content">
                    <div class="speed-item">
                        <span class="speed-label">Motor Speed</span>
                        <div class="speed-control">
                            <input type="range" min="0" max="100" value="100" class="speed-slider" id="motorSpeed" oninput="updateMotorSpeed(this.value)">
                            <span class="speed-value" id="motorSpeedValue">100%</span>
                        </div>
                    </div>
                    <div class="speed-item">
                        <span class="speed-label">Servo Speed</span>
                        <div class="speed-control">
                            <input type="range" min="1" max="20" value="10" class="speed-slider" id="servoSpeed" oninput="updateServoSpeed(this.value)">
                            <span class="speed-value" id="servoSpeedValue">10&#176;/step</span>
                        </div>
                    </div>
                </div>
            </div>
        </div>
        
        <div class="theme-toggle" id="themeToggle" onclick="toggleTheme()">
            <div class="theme-icon">&#9728;</div>
            <div class="slider"></div>
            <div class="theme-icon">&#127769;</div>
        </div>
    </div>
    
    <div class="status-container">
        <div class="status-box">
            <div class="status-title">Distance</div>
            <div class="status-value" id="distance">-- cm</div>
        </div>
        <div class="status-box">
            <div class="status-title">Battery</div>
            <div class="status-value" id="battery">-- %</div>
        </div>
    </div>
    
    <div class="camera-container">
        <img src="" id="photo" class="camera-image" alt="Robot Camera">
    </div>
    
    <div class="controls">
        <div class="control-group">
            <div class="direction-controls">
                <button class="btn btn-forward" id="forwardBtn" 
                    ontouchstart="handleStart(event, 'forward');" 
                    ontouchend="handleEnd(event);"
                    onmousedown="handleStart(event, 'forward');" 
                    onmouseup="handleEnd(event);"
                    onmouseleave="handleEnd(event);">Forward</button>
                <button class="btn btn-left" id="leftBtn" 
                    ontouchstart="handleStart(event, 'left');" 
                    ontouchend="handleEnd(event);"
                    onmousedown="handleStart(event, 'left');" 
                    onmouseup="handleEnd(event);"
                    onmouseleave="handleEnd(event);">Left</button>
                <button class="btn btn-light" onclick="toggleLight();">Light <span id="lightIndicator" class="light-indicator"></span></button>
                <button class="btn btn-right" id="rightBtn" 
                    ontouchstart="handleStart(event, 'right');" 
                    ontouchend="handleEnd(event);"
                    onmousedown="handleStart(event, 'right');" 
                    onmouseup="handleEnd(event);"
                    onmouseleave="handleEnd(event);">Right</button>
                <button class="btn btn-backward" id="backwardBtn" 
                    ontouchstart="handleStart(event, 'backward');" 
                    ontouchend="handleEnd(event);"
                    onmousedown="handleStart(event, 'backward');" 
                    onmouseup="handleEnd(event);"
                    onmouseleave="handleEnd(event);">Backward</button>
            </div>
        </div>
        
        <div class="control-group">
            <div class="control-title">Camera Tilt</div>
            <div class="servo-controls">
                <button class="btn btn-servo-minus" 
                    ontouchstart="handleServoStart(event, 'servo-');" 
                    ontouchend="handleServoEnd(event);"
                    onmousedown="handleServoStart(event, 'servo-');" 
                    onmouseup="handleServoEnd(event);">-</button>
                <div class="servo-display"><span id="servoAngle">90</span>&#176;</div>
                <button class="btn btn-servo-plus" 
                    ontouchstart="handleServoStart(event, 'servo+');" 
                    ontouchend="handleServoEnd(event);"
                    onmousedown="handleServoStart(event, 'servo+');" 
                    onmouseup="handleServoEnd(event);">+</button>
            </div>
        </div>
        
        <div class="control-group">
            <div class="control-title">Camera Pan</div>
            <div class="servo-controls">
                <button class="btn btn-servo-minus" 
                    ontouchstart="handleServoStart(event, 'servo2-');" 
                    ontouchend="handleServoEnd(event);"
                    onmousedown="handleServoStart(event, 'servo2-');" 
                    onmouseup="handleServoEnd(event);">-</button>
                <div class="servo-display"><span id="servo2Angle">90</span>&#176;</div>
                <button class="btn btn-servo-plus" 
                    ontouchstart="handleServoStart(event, 'servo2+');" 
                    ontouchend="handleServoEnd(event);"
                    onmousedown="handleServoStart(event, 'servo2+');" 
                    onmouseup="handleServoEnd(event);">+</button>
            </div>
        </div>
    </div>
    
    <script>
        var commandInterval;
        var servoInterval;
        var currentAngle = 90;
        var currentAngle2 = 0;
        var lightOn = false;
        var motorSpeed = 100;    // Default motor speed (0-100%)
        var servoSpeed = 10;    // Default servo speed (1-20 degrees per step)
        var movementActive = false;
        var currentDirection = '';
        
        // Theme toggle functionality
        const htmlElement = document.documentElement;
        
        // Check for saved theme preference or default to light
        const currentTheme = localStorage.getItem('theme') || 'light';
        htmlElement.setAttribute('data-theme', currentTheme);
        
        // Update toggle position based on current theme
        updateThemeToggle(currentTheme);
        
        function toggleTheme() {
            const currentTheme = htmlElement.getAttribute('data-theme');
            const newTheme = currentTheme === 'light' ? 'dark' : 'light';
            
            htmlElement.setAttribute('data-theme', newTheme);
            localStorage.setItem('theme', newTheme);
            
            // Update toggle position
            updateThemeToggle(newTheme);
        }
        
        function updateThemeToggle(theme) {
            const slider = document.querySelector('.theme-toggle .slider');
            if (theme === 'dark') {
                slider.style.transform = 'translateX(30px)';
            } else {
                slider.style.transform = 'translateX(0)';
            }
        }
        
        // Settings dropdown functionality
        function toggleSettings() {
            const dropdown = document.getElementById("settingsDropdown");
            dropdown.classList.toggle("show");
            
            // Close speed dropdown if open
            document.getElementById("speedDropdown").classList.remove("show");
            
            // Add overlay background
            if (dropdown.classList.contains("show")) {
                createOverlay();
            } else {
                removeOverlay();
            }
        }
        
        // Speed settings dropdown functionality
        function toggleSpeedSettings() {
            const dropdown = document.getElementById("speedDropdown");
            dropdown.classList.toggle("show");
            
            // Close settings dropdown if open
            document.getElementById("settingsDropdown").classList.remove("show");
            
            // Add overlay background
            if (dropdown.classList.contains("show")) {
                createOverlay();
            } else {
                removeOverlay();
            }
        }
        
        // Create overlay background
        function createOverlay() {
            // Check if overlay already exists
            if (document.getElementById("dropdownOverlay")) {
                return;
            }
            
            const overlay = document.createElement("div");
            overlay.id = "dropdownOverlay";
            overlay.style.position = "fixed";
            overlay.style.top = "0";
            overlay.style.left = "0";
            overlay.style.width = "100%";
            overlay.style.height = "100%";
            overlay.style.backgroundColor = "rgba(0, 0, 0, 0.5)";
            overlay.style.zIndex = "999";
            overlay.onclick = closeAllDropdowns;
            
            document.body.appendChild(overlay);
        }
        
        // Remove overlay background
        function removeOverlay() {
            const overlay = document.getElementById("dropdownOverlay");
            if (overlay) {
                overlay.remove();
            }
        }
        
        // Close all dropdowns
        function closeAllDropdowns() {
            document.getElementById("settingsDropdown").classList.remove("show");
            document.getElementById("speedDropdown").classList.remove("show");
            removeOverlay();
        }
        
        // Camera settings functionality
        function updateCameraSetting(setting, value) {
            // Update the displayed value
            const valueElement = document.getElementById(setting + 'Value');
            if (valueElement) {
                valueElement.textContent = value;
            }
            
            // Send the setting to the server
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/camera?setting=" + setting + "&value=" + value, true);
            xhr.send();
        }
        
        // Reset camera settings to default
        function resetCameraSettings() {
            // Reset all sliders to default value (0)
            const sliders = ['brightness', 'contrast', 'saturation', 'sharpness', 'exposure'];
            sliders.forEach(slider => {
                document.getElementById(slider).value = 0;
                document.getElementById(slider + 'Value').textContent = '0';
                updateCameraSetting(slider, 0);
            });
            
            // Reset dropdowns to default value (0)
            document.getElementById('specialEffect').value = 0;
            document.getElementById('whiteBalance').value = 0;
            updateCameraSetting('special_effect', 0);
            updateCameraSetting('wb_mode', 0);
            
            // Send reset command to server
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/camera?reset=true", true);
            xhr.send();
        }
        
        // Speed control functions
        function updateMotorSpeed(value) {
            motorSpeed = parseInt(value);
            document.getElementById('motorSpeedValue').textContent = motorSpeed + '%';
            
            // Send the new motor speed to the server
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/action?motorSpeed=" + motorSpeed, true);
            xhr.send();
        }
        
        function updateServoSpeed(value) {
            servoSpeed = parseInt(value);
            document.getElementById('servoSpeedValue').textContent = servoSpeed + '&#176;/step';
            
            // Send the new servo speed to the server
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/action?servoSpeed=" + servoSpeed, true);
            xhr.send();
        }
        
        // Movement control functions
        function handleStart(event, direction) {
            // Prevent default behavior to avoid vibration
            event.preventDefault();
            
            // Clear any existing interval
            if (commandInterval) {
                clearInterval(commandInterval);
            }
            
            movementActive = true;
            currentDirection = direction;
            
            // Send the movement command immediately
            sendCommand(direction);
            
            // Set up interval to keep sending the command every 50ms while button is pressed
            commandInterval = setInterval(function() {
                sendCommand(direction);
            }, 50); // Reduced interval for more responsive control
        }
        
        function handleEnd(event) {
            // Prevent default behavior to avoid vibration
            event.preventDefault();
            
            if (!movementActive) return;
            
            movementActive = false;
            
            // Clear the interval
            if (commandInterval) {
                clearInterval(commandInterval);
                commandInterval = null;
            }
            
            // Do NOT send stop command - the other side will handle it
        }
        
        // Servo control functions
        function handleServoStart(event, direction) {
            // Prevent default behavior to avoid vibration
            event.preventDefault();
            
            // Clear any existing interval
            if (servoInterval) {
                clearInterval(servoInterval);
            }
            
            // Send command immediately
            sendCommand(direction);
            
            // Set up interval to keep sending the command
            servoInterval = setInterval(function() {
                sendCommand(direction);
            }, 100);
        }
        
        function handleServoEnd(event) {
            // Prevent default behavior to avoid vibration
            event.preventDefault();
            
            // Clear the interval
            if (servoInterval) {
                clearInterval(servoInterval);
                servoInterval = null;
            }
        }
        
        function sendCommand(cmd) {
            var fullCommand;
            
            // Handle movement commands - send just the direction
            if (cmd === 'forward' || cmd === 'backward' || cmd === 'left' || cmd === 'right') {
                fullCommand = cmd;   // Just send the command without speed
            } 
            // Handle servo commands - send increment/decrement
            else if (cmd === 'servo+' || cmd === 'servo-' || cmd === 'servo2+' || cmd === 'servo2-') {
                // Calculate new angle
                if (cmd === 'servo+') {
                    currentAngle = Math.min(currentAngle + servoSpeed, 180);
                    fullCommand = "servo1:" + currentAngle;
                } else if (cmd === 'servo-') {
                    currentAngle = Math.max(currentAngle - servoSpeed, 0);
                    fullCommand = "servo1:" + currentAngle;
                } else if (cmd === 'servo2+') {
                    currentAngle2 = Math.min(currentAngle2 + servoSpeed, 180);
                    fullCommand = "servo2:" + currentAngle2;
                } else if (cmd === 'servo2-') {
                    currentAngle2 = Math.max(currentAngle2 - servoSpeed, 0);
                    fullCommand = "servo2:" + currentAngle2;
                }
                
                // Update the display
                document.getElementById('servoAngle').innerText = currentAngle;
                document.getElementById('servo2Angle').innerText = currentAngle2;
            } 
            // Other commands (like LED) remain unchanged
            else {
                fullCommand = cmd;
            }
            
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/action?go=" + fullCommand, true);
            xhr.send();
        }
        
        function toggleLight() {
            lightOn = !lightOn;
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "/action?go=led", true);
            xhr.send();
            
            // Update light indicator
            var indicator = document.getElementById('lightIndicator');
            if (lightOn) {
                indicator.classList.add('light-on');
            } else {
                indicator.classList.remove('light-on');
            }
        }
        
        window.onload = function() {
            document.getElementById("photo").src = window.location.href.slice(0, -1) + ":81/stream";
            
            // Initialize speed displays
            updateMotorSpeed(motorSpeed);
            updateServoSpeed(servoSpeed);
            
            // Update distance every 500ms
            setInterval(function() {
                var xhr = new XMLHttpRequest();
                xhr.onreadystatechange = function() {
                    if (xhr.readyState == 4 && xhr.status == 200) {
                        document.getElementById("distance").innerHTML = xhr.responseText + " cm";
                    }
                };
                xhr.open("GET", "/distance", true);
                xhr.send();
            }, 500);
            
            // Update battery every 2000ms
            setInterval(function() {
                var xhr = new XMLHttpRequest();
                xhr.onreadystatechange = function() {
                    if (xhr.readyState == 4 && xhr.status == 200) {
                        document.getElementById("battery").innerHTML = xhr.responseText + " %";
                    }
                };
                xhr.open("GET", "/battery", true);
                xhr.send();
            }, 2000);
        }
    </script>
</body>
</html>
)rawliteral";

#endif // HTML_CONTENT_H