<template>
    <h2 class="text-center">HTTP</h2>
    <div class="flex flex-wrap gap-2 w-fit">
        <PieChart v-for="(chartOptions, index) in chartOptionses" :key="index" :chartOptions="chartOptions" :chartSeries="chartSerieses[index]" />
    </div>
</template>

<script>
import PieChart from '../components/PieChart.vue';
export default {
    name: 'L7View',
    components: {
        PieChart,
    },
    data() {
        return {
            chartOptionses: [],
            chartSerieses: [],
        };
    },
    mounted() {
        setInterval(() => {
            fetch('http://work.isk:8000/api/stats/http')
                .then(response => response.json())
                .then(data => {
                    let fields = ["contentTypeCounter", "hostCounter", "methodCounter", "statusCounter", "userAgentCounter"];
                    this.chartOptionses = fields.map(field => {
                        return {
                            chart: {
                                id: field,
                            },
                            labels: Object.keys(data[field]) || [],
                            title: {
                                text: field,
                            },
                        };
                    });
                    this.chartSerieses = fields.map(field => {
                        return Object.values(data[field]) || [];
                    });
                });
        }, 1000);
    },
}
</script>
