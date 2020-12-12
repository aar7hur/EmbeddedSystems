import matplotlib.pyplot as plt
import pandas as pd

class PlotGraphic:
    """
    Class used to plot graphics
    """
    @staticmethod
    def createGraphic(data, title: str, xlabel: str, ylabel: str, label: str):
        """
        Method that create and plots graphic.
        :param data: data used to be plotted. Must be pandas object
        :param titte: image tittle
        :param xlabel: variable magnitute. Time, Voltage...
        :param ylabel: variable magnitute. Time, Voltage, temperature..
        :param label: label to index in image
        """
        plt.style.use('bmh')
        plt.rcParams['animation.html'] = 'jshtml'
        plt.rcParams["figure.figsize"] = (10,8)
        
        xAxis = data.index
        fig, ax0 = plt.subplots(nrows = 1, constrained_layout = True)

        ax0.set_title(title)
        ax0.set_xlabel(xlabel)
        ax0.set_ylabel(ylabel)
        line,  = ax0.plot(xAxis, data,
                        color='r',
                        label=label)
        ax0.legend()
        plt.show()


if __name__ == '__main__':
    import pandas
    
    data = {
        'Processo número': [1,2,3,4,5,10,15,20,25,35,45,46,47,48,49,50,60,70,80],
        'Tempo': [2.004, 1.015, 0.675, 0.516, 0.401, 0.206, 0.149, 0.115, 0.107, 0.110, 0.084, 0.101, 0.113, 0.098, 0.076, 0.088, 0.061, 0.001, 0.004]
    }
    df = pd.DataFrame(data)

    df.plot(x='Processo número', y='Tempo',
            title='Gráfico de processos em função do tempo de execução', ylabel='Tempo[s]', label='Fatorial de 1700')
    plt.show()

   



