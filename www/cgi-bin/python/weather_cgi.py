#!/usr/bin/env python3
import os
import sys
import csv
import openmeteo_requests
import requests_cache
from retry_requests import retry
from urllib.parse import parse_qs

# Setup the Open-Meteo API client with cache and retry on error
cache_session = requests_cache.CachedSession('.cache', expire_after=3600)
retry_session = retry(cache_session, retries=5, backoff_factor=0.2)
openmeteo = openmeteo_requests.Client(session=retry_session)

def read_csv_data(csv_file):
    """Lit le fichier CSV et retourne une liste de dictionnaires"""
    data = []
    try:
        with open(csv_file, 'r', encoding='utf-8') as f:
            reader = csv.reader(f)
            for row in reader:
                if len(row) == 4:
                    data.append({
                        'country': row[0].strip(),
                        'capital': row[1].strip(),
                        'latitude': float(row[2]),
                        'longitude': float(row[3])
                    })
    except Exception as e:
        return None, str(e)
    return data, None

def find_location(data, search_term):
    """Cherche une ville ou un pays dans les données"""
    search_lower = search_term.lower()
    for entry in data:
        if (search_lower == entry['country'].lower() or 
            search_lower == entry['capital'].lower()):
            return entry
    return None

def get_weather_data(latitude, longitude):
    """Récupère les données météo via l'API Open-Meteo"""
    url = "https://api.open-meteo.com/v1/forecast"
    params = {
        "latitude": latitude,
        "longitude": longitude,
        "current": ["apparent_temperature", "relative_humidity_2m", "wind_speed_10m", 
                   "wind_direction_10m", "wind_gusts_10m", "precipitation", "rain", 
                   "showers", "snowfall", "weather_code", "cloud_cover", "pressure_msl", 
                   "surface_pressure"],
        "timezone": "auto",
        "forecast_days": 1
    }
    
    try:
        responses = openmeteo.weather_api(url, params=params)
        return responses[0], None
    except Exception as e:
        return None, str(e)

def generate_error_html(error_msg):
    """Génère une page d'erreur HTML"""
    print("Content-Type: text/html; charset=utf-8")
    print()
    print(f"""<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Erreur - Système</title>
    <style>
        * {{
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }}
        
        @keyframes stars {{
            from {{ transform: translateY(0px); }}
            to {{ transform: translateY(-2000px); }}
        }}
        
        @keyframes glitch {{
            0%, 100% {{ transform: translate(0); }}
            20% {{ transform: translate(-2px, 2px); }}
            40% {{ transform: translate(-2px, -2px); }}
            60% {{ transform: translate(2px, 2px); }}
            80% {{ transform: translate(2px, -2px); }}
        }}
        
        body {{
            font-family: 'Courier New', monospace;
            background: #000000;
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
            position: relative;
            overflow: hidden;
        }}
        
        .stars {{
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 200%;
            background-image: 
                radial-gradient(2px 2px at 20px 30px, #eee, transparent),
                radial-gradient(2px 2px at 60px 70px, #fff, transparent),
                radial-gradient(1px 1px at 50px 50px, #ddd, transparent);
            background-size: 200px 200px;
            animation: stars 100s linear infinite;
            z-index: 0;
        }}
        
        .container {{
            position: relative;
            z-index: 10;
            max-width: 600px;
            background: rgba(10, 10, 30, 0.85);
            border: 1px solid rgba(255, 0, 0, 0.5);
            padding: 40px;
            box-shadow: 0 0 40px rgba(255, 0, 0, 0.3);
            backdrop-filter: blur(10px);
        }}
        
        .error {{
            text-align: center;
        }}
        
        .error-code {{
            font-size: 72px;
            color: #ff0000;
            margin-bottom: 20px;
            text-shadow: 0 0 20px rgba(255, 0, 0, 0.8);
            animation: glitch 1s infinite;
        }}
        
        h1 {{
            color: #ff4444;
            font-size: 24px;
            margin-bottom: 20px;
            text-transform: uppercase;
            letter-spacing: 4px;
        }}
        
        p {{
            color: rgba(255, 255, 255, 0.7);
            font-size: 14px;
            line-height: 1.8;
            margin-bottom: 30px;
        }}
        
        a {{
            display: inline-block;
            padding: 12px 30px;
            background: linear-gradient(90deg, 
                rgba(255, 0, 0, 0.6) 0%, 
                rgba(200, 0, 0, 0.6) 100%);
            color: #fff;
            text-decoration: none;
            border: 1px solid #ff0000;
            text-transform: uppercase;
            letter-spacing: 2px;
            font-size: 12px;
            transition: all 0.3s ease;
        }}
        
        a:hover {{
            box-shadow: 0 0 20px rgba(255, 0, 0, 0.6);
            transform: translateY(-2px);
        }}
    </style>
</head>
<body>
    <div class="stars"></div>
    <div class="container">
        <div class="error">
            <div class="error-code">ERROR</div>
            <h1>Échec de transmission</h1>
            <p>{error_msg}</p>
            <a href="/weather_form.html">RETOUR AU SYSTÈME</a>
        </div>
    </div>
</body>
</html>""")

def generate_weather_html(location, weather_response):
    """Génère la page HTML avec les résultats météo"""
    
    # Headers CGI
    print("Content-Type: text/html; charset=utf-8")
    print()
    
    current = weather_response.Current()
    
    # Récupération des données
    temp = current.Variables(0).Value()
    humidity = current.Variables(1).Value()
    wind_speed = current.Variables(2).Value()
    wind_direction = current.Variables(3).Value()
    wind_gusts = current.Variables(4).Value()
    precipitation = current.Variables(5).Value()
    rain = current.Variables(6).Value()
    showers = current.Variables(7).Value()
    snowfall = current.Variables(8).Value()
    weather_code = current.Variables(9).Value()
    cloud_cover = current.Variables(10).Value()
    pressure_msl = current.Variables(11).Value()
    surface_pressure = current.Variables(12).Value()
    
    # Description météo
    weather_descriptions = {
        0: "CIEL DEGAGE",
        1: "PRINCIPALEMENT DEGAGE", 2: "PARTIELLEMENT NUAGEUX", 3: "COUVERT",
        45: "BROUILLARD", 48: "BROUILLARD GIVRANT",
        51: "BRUINE LEGERE", 53: "BRUINE MODEREE", 55: "BRUINE DENSE",
        61: "PLUIE LEGERE", 63: "PLUIE MODEREE", 65: "PLUIE FORTE",
        71: "NEIGE LEGERE", 73: "NEIGE MODEREE", 75: "NEIGE FORTE",
        80: "AVERSES LEGERES", 81: "AVERSES MODEREES", 82: "AVERSES VIOLENTES",
        95: "ORAGE", 96: "ORAGE AVEC GRELE", 99: "ORAGE VIOLENT"
    }
    weather_desc = weather_descriptions.get(int(weather_code), "CONDITIONS VARIABLES")
    
    print(f"""<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Données Atmosphériques - {location['capital']}</title>
    <style>
        * {{
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }}
        
        @keyframes stars {{
            from {{ transform: translateY(0px); }}
            to {{ transform: translateY(-2000px); }}
        }}
        
        @keyframes rotate {{
            from {{ transform: rotate(0deg); }}
            to {{ transform: rotate(360deg); }}
        }}
        
        @keyframes pulse {{
            0%, 100% {{ opacity: 0.8; transform: scale(1); }}
            50% {{ opacity: 1; transform: scale(1.02); }}
        }}
        
        @keyframes dataStream {{
            0% {{ opacity: 0; transform: translateY(20px); }}
            100% {{ opacity: 1; transform: translateY(0); }}
        }}
        
        body {{
            font-family: 'Courier New', monospace;
            background: #000000;
            min-height: 100vh;
            padding: 30px 20px;
            position: relative;
            overflow-x: hidden;
        }}
        
        .stars {{
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 200%;
            background-image: 
                radial-gradient(2px 2px at 20px 30px, #eee, transparent),
                radial-gradient(2px 2px at 60px 70px, #fff, transparent),
                radial-gradient(1px 1px at 50px 50px, #ddd, transparent),
                radial-gradient(1px 1px at 130px 80px, #fff, transparent),
                radial-gradient(2px 2px at 90px 10px, #fff, transparent);
            background-size: 200px 200px;
            animation: stars 100s linear infinite;
            z-index: 0;
        }}
        
        .container {{
            position: relative;
            z-index: 10;
            max-width: 1200px;
            margin: 0 auto;
            background: rgba(10, 10, 30, 0.85);
            border: 1px solid rgba(0, 150, 255, 0.3);
            padding: 40px;
            box-shadow: 
                0 0 40px rgba(0, 100, 200, 0.3),
                inset 0 0 60px rgba(0, 0, 0, 0.5);
            backdrop-filter: blur(10px);
            animation: dataStream 0.8s ease-out;
        }}
        
        .header {{
            text-align: center;
            margin-bottom: 40px;
            padding-bottom: 30px;
            border-bottom: 1px solid rgba(0, 150, 255, 0.3);
        }}
        
        .status {{
            color: #00ff00;
            font-size: 12px;
            letter-spacing: 3px;
            margin-bottom: 20px;
            text-transform: uppercase;
        }}
        
        .location {{
            margin-bottom: 20px;
        }}
        
        h1 {{
            color: #00d4ff;
            font-size: 42px;
            margin-bottom: 8px;
            text-transform: uppercase;
            letter-spacing: 6px;
            text-shadow: 0 0 20px rgba(0, 212, 255, 0.8);
            font-weight: 300;
        }}
        
        .country {{
            color: rgba(255, 255, 255, 0.6);
            font-size: 18px;
            letter-spacing: 3px;
            text-transform: uppercase;
        }}
        
        .coordinates {{
            color: rgba(0, 212, 255, 0.5);
            font-size: 12px;
            margin-top: 15px;
            letter-spacing: 2px;
        }}
        
        .main-display {{
            text-align: center;
            margin: 40px 0;
            padding: 30px;
            background: rgba(0, 50, 100, 0.2);
            border: 1px solid rgba(0, 150, 255, 0.2);
        }}
        
        .temperature {{
            font-size: 72px;
            color: #00d4ff;
            font-weight: 300;
            margin: 20px 0;
            text-shadow: 0 0 30px rgba(0, 212, 255, 0.8);
            letter-spacing: -2px;
        }}
        
        .weather-desc {{
            font-size: 16px;
            color: rgba(255, 255, 255, 0.7);
            letter-spacing: 3px;
            text-transform: uppercase;
        }}
        
        .data-grid {{
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
            gap: 20px;
            margin-top: 40px;
        }}
        
        .data-card {{
            background: rgba(0, 30, 60, 0.4);
            border: 1px solid rgba(0, 150, 255, 0.3);
            padding: 20px;
            transition: all 0.3s ease;
            position: relative;
            overflow: hidden;
        }}
        
        .data-card::before {{
            content: '';
            position: absolute;
            top: 0;
            left: -100%;
            width: 100%;
            height: 1px;
            background: linear-gradient(90deg, transparent, #00d4ff, transparent);
            transition: left 0.5s;
        }}
        
        .data-card:hover::before {{
            left: 100%;
        }}
        
        .data-card:hover {{
            background: rgba(0, 50, 100, 0.5);
            border-color: #00d4ff;
            box-shadow: 0 0 25px rgba(0, 212, 255, 0.3);
            transform: translateY(-3px);
        }}
        
        .data-label {{
            font-size: 11px;
            color: rgba(0, 212, 255, 0.7);
            margin-bottom: 12px;
            text-transform: uppercase;
            letter-spacing: 2px;
        }}
        
        .data-value {{
            font-size: 32px;
            color: #ffffff;
            font-weight: 300;
            letter-spacing: -1px;
        }}
        
        .data-unit {{
            font-size: 14px;
            color: rgba(255, 255, 255, 0.5);
            margin-left: 4px;
        }}
        
        .data-bar {{
            margin-top: 10px;
            height: 2px;
            background: rgba(0, 100, 200, 0.3);
            position: relative;
        }}
        
        .data-bar-fill {{
            position: absolute;
            left: 0;
            top: 0;
            height: 100%;
            background: linear-gradient(90deg, #0064c8, #00d4ff);
            box-shadow: 0 0 10px rgba(0, 212, 255, 0.6);
        }}
        
        .footer {{
            text-align: center;
            margin-top: 50px;
            padding-top: 30px;
            border-top: 1px solid rgba(0, 150, 255, 0.3);
        }}
        
        .back-button {{
            display: inline-block;
            padding: 12px 30px;
            background: linear-gradient(90deg, 
                rgba(0, 100, 200, 0.8) 0%, 
                rgba(0, 150, 255, 0.8) 50%,
                rgba(0, 100, 200, 0.8) 100%);
            color: #fff;
            text-decoration: none;
            border: 1px solid #00d4ff;
            font-size: 14px;
            text-transform: uppercase;
            letter-spacing: 2px;
            transition: all 0.3s ease;
            position: relative;
            overflow: hidden;
        }}
        
        .back-button::before {{
            content: '';
            position: absolute;
            top: 0;
            left: -100%;
            width: 100%;
            height: 100%;
            background: linear-gradient(90deg, 
                transparent, 
                rgba(255, 255, 255, 0.3), 
                transparent);
            transition: left 0.5s;
        }}
        
        .back-button:hover::before {{
            left: 100%;
        }}
        
        .back-button:hover {{
            box-shadow: 0 0 30px rgba(0, 212, 255, 0.6);
            transform: translateY(-2px);
        }}
    </style>
</head>
<body>
    <div class="stars"></div>
    
    <div class="container">
        <div class="header">
            <div class="status">[ DONNÉES REÇUES ]</div>
            <div class="location">
                <h1>{location['capital']}</h1>
                <div class="country">{location['country']}</div>
                <div class="coordinates">COORDONNÉES: {location['latitude']}° / {location['longitude']}°</div>
            </div>
        </div>
        
        <div class="main-display">
            <div class="temperature">{temp:.1f}°C</div>
            <div class="weather-desc">{weather_desc}</div>
        </div>
        
        <div class="data-grid">
            <div class="data-card">
                <div class="data-label">Humidité</div>
                <div class="data-value">{humidity:.0f}<span class="data-unit">%</span></div>
                <div class="data-bar">
                    <div class="data-bar-fill" style="width: {humidity}%"></div>
                </div>
            </div>
            
            <div class="data-card">
                <div class="data-label">Vitesse du vent</div>
                <div class="data-value">{wind_speed:.1f}<span class="data-unit">km/h</span></div>
                <div class="data-bar">
                    <div class="data-bar-fill" style="width: {min(wind_speed, 100)}%"></div>
                </div>
            </div>
            
            <div class="data-card">
                <div class="data-label">Rafales</div>
                <div class="data-value">{wind_gusts:.1f}<span class="data-unit">km/h</span></div>
                <div class="data-bar">
                    <div class="data-bar-fill" style="width: {min(wind_gusts, 100)}%"></div>
                </div>
            </div>
            
            <div class="data-card">
                <div class="data-label">Direction du vent</div>
                <div class="data-value">{wind_direction:.0f}<span class="data-unit">°</span></div>
                <div class="data-bar">
                    <div class="data-bar-fill" style="width: {(wind_direction / 360) * 100}%"></div>
                </div>
            </div>
            
            <div class="data-card">
                <div class="data-label">Couverture nuageuse</div>
                <div class="data-value">{cloud_cover:.0f}<span class="data-unit">%</span></div>
                <div class="data-bar">
                    <div class="data-bar-fill" style="width: {cloud_cover}%"></div>
                </div>
            </div>
            
            <div class="data-card">
                <div class="data-label">Précipitations</div>
                <div class="data-value">{precipitation:.1f}<span class="data-unit">mm</span></div>
                <div class="data-bar">
                    <div class="data-bar-fill" style="width: {min(precipitation * 10, 100)}%"></div>
                </div>
            </div>
            
            <div class="data-card">
                <div class="data-label">Pression (niveau mer)</div>
                <div class="data-value">{pressure_msl:.0f}<span class="data-unit">hPa</span></div>
                <div class="data-bar">
                    <div class="data-bar-fill" style="width: {((pressure_msl - 950) / 100) * 100}%"></div>
                </div>
            </div>
            
            <div class="data-card">
                <div class="data-label">Pression (surface)</div>
                <div class="data-value">{surface_pressure:.0f}<span class="data-unit">hPa</span></div>
                <div class="data-bar">
                    <div class="data-bar-fill" style="width: {((surface_pressure - 950) / 100) * 100}%"></div>
                </div>
            </div>
        </div>
        
        <div class="footer">
            <a href="/weather_form.html" class="back-button">NOUVEAU SCAN</a>
        </div>
    </div>
</body>
</html>""")

def main():
    # Récupérer le paramètre GET depuis QUERY_STRING
    query_string = os.environ.get('QUERY_STRING', '')
    params = parse_qs(query_string)
    
    city = params.get('city', [''])[0].strip()
    
    # Si pas de paramètre, rediriger vers le formulaire
    if not city:
        print("Status: 302 Found")
        print("Location: /weather_form.html")
        print()
        return
    
    # Trouver le chemin du CSV (même dossier que le script)
    script_dir = os.path.dirname(os.path.abspath(__file__))
    csv_file = os.path.join(script_dir, 'filtered_capitals.csv')
    
    # Lire les données CSV
    data, error = read_csv_data(csv_file)
    if error:
        generate_error_html(f"Erreur lors de la lecture du fichier CSV : {error}")
        return
    
    # Chercher la localisation
    location = find_location(data, city)
    if not location:
        generate_error_html(f"Cible '{city}' introuvable dans la base de données. Vérifiez les coordonnées.")
        return
    
    # Récupérer les données météo
    weather_response, error = get_weather_data(location['latitude'], location['longitude'])
    if error:
        generate_error_html(f"Échec de connexion au satellite météo : {error}")
        return
    
    # Générer la page HTML avec les résultats
    generate_weather_html(location, weather_response)

if __name__ == "__main__":
    main()
