import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Plot State-Machine Output
df1 = pd.read_csv('Tests/Output/EffectStateMachine.csv')
plt.plot(df1['cycle'], df1['value'], label='longRun')
plt.xlabel('iteration')
plt.ylabel('intensity')
plt.title('LongRun')
plt.legend()
plt.show()

# Plot Processor Output
df2 = pd.read_csv('Tests/Output/EffectProcessorRun.csv')
plt.plot(df2['cycle'], df2['R'], color='red')
plt.plot(df2['cycle'], df2['G'], color='green')
plt.plot(df2['cycle'], df2['B'], color='blue')
plt.plot(df2['cycle'], df2['RGB'], color='grey')
plt.show()
