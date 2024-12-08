import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates

# Datos del proyecto
data = {
    "Tarea": [
        "Estudio de ideas y repaso de conceptos adquiridos",
        "Primer diseño y pruebas (FM a varactor)",
        "Desarrollo del segundo diseño (Receptor superheterodino FM)",
        "Desarrollo del primer diseño factible y redacción (Par Darlington con conversión directa)",
        "Estudio de conceptos avanzados y diseño final",
        "Desarrollo del diseño final y redacción (Receptor super-regenerativo)"
    ],
    "Inicio": [
        "2023-06-01",
        "2023-09-01",
        "2023-12-01",
        "2024-04-01",
        "2024-06-01",
        "2024-09-01"
    ],
    "Fin": [
        "2023-09-30",
        "2023-10-31",
        "2024-04-30",
        "2024-06-30",
        "2024-09-30",
        "2024-12-31"
    ]
}

# Convertir datos a DataFrame
df = pd.DataFrame(data)
df['Inicio'] = pd.to_datetime(df['Inicio'])
df['Fin'] = pd.to_datetime(df['Fin'])

# Configurar el gráfico Gantt
fig, ax = plt.subplots(figsize=(12, 6))
for i, row in df.iterrows():
    ax.barh(row['Tarea'], (row['Fin'] - row['Inicio']).days, left=row['Inicio'], align='center')

# Formatear el eje X
ax.xaxis.set_major_locator(mdates.MonthLocator(interval=3))
ax.xaxis.set_major_formatter(mdates.DateFormatter('%b %Y'))
plt.xticks(rotation=45)

# Etiquetas y título
ax.set_xlabel("Tiempo")
ax.set_ylabel("Tareas")
ax.set_title("Diagrama de Gantt - Proyecto de 2 años")
plt.tight_layout()

# Mostrar el gráfico
plt.show()
