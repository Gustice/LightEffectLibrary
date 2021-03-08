import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Plot State-Machine Output
df1 = pd.read_csv('WaveFormGenerator/Output/gau8_fullPulse_Plot.csv')
df2 = pd.read_csv('WaveFormGenerator/Output/gau8_fullSlope_Plot.csv')
df3 = pd.read_csv('WaveFormGenerator/Output/gau8_initSlope_Plot.csv')
df4 = pd.read_csv('WaveFormGenerator/Output/gau8_offsetPulse_Plot.csv')
df5 = pd.read_csv('WaveFormGenerator/Output/gau8_offsetSlope_Plot.csv')

plt.plot(df1['Cycl'], df1['value'], label='fullPulse')
plt.plot(df2['Cycl'], df2['value'], label='fullSlope')
plt.plot(df3['Cycl'], df3['value'], label='initSlope')
plt.plot(df4['Cycl'], df4['value'], label='offsetPulse')
plt.plot(df5['Cycl'], df5['value'], label='offsetSlope')

plt.xlabel('iteration')
plt.ylabel('intensity')
plt.title('Waveforms')
plt.legend()
plt.show()
