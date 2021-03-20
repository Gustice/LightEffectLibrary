import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Plot State-Machine Output
df0 = pd.read_csv('Tests/Output/cEffectStateMachine.csv')
df1 = pd.read_csv('Tests/Output/EffectStateMachine.csv')
df2 = pd.read_csv('Tests/Output/EffectSequenceStepper.csv')
plt.plot(df0['cycle'], df0['value'], label='cStyle')
plt.plot(df1['cycle'], df1['value'], label='c++')
plt.plot(df2['cycle'], df2['value'], label='c++ alt')
plt.xlabel('iteration')
plt.ylabel('intensity')
plt.title('LongRun')
plt.legend()
plt.show()

# Plot Processor Output
df3 = pd.read_csv('Tests/Output/EffectProcessorRun.csv')
plt.plot(df3['cycle'], df3['R'], color='red')
plt.plot(df3['cycle'], df3['G'], color='green')
plt.plot(df3['cycle'], df3['B'], color='blue')
plt.plot(df3['cycle'], df3['RGB'], color='grey')
plt.show()

df4 = pd.read_csv('Tests/Output/EffectSequencerRun.csv')
plt.plot(df4['cycle'], df4['R'], color='red')
plt.plot(df4['cycle'], df4['G'], color='green')
plt.plot(df4['cycle'], df4['B'], color='blue')
plt.plot(df4['cycle'], df4['RGB'], color='grey')
plt.show()
